#!/usr/bin/env python3
"""
generate_sid_audio.py
Synthesizes authentic Commodore 64 MOS 6581 SID waveforms to replace:
1. All instruments/samples in base/music/*.it (Title, Outdoors/Stage 1, Indoors, Highscore)
2. All sound effects in base/audio/*.wav (Jump, Ding, Crack, Disk, Stomp, Shoot, Morphing, Died, Click, Portal)
"""

import struct
import numpy as np
import wave
import math
import os

# ==========================================
# 1. MOS 6581 SID SYNTHESIS PRIMITIVES
# ==========================================

def sid_lfsr_noise(num_samples):
    """Simulate 23-bit Commodore 64 MOS 6581 LFSR noise generator."""
    reg = 0x7FFFF8
    out = np.zeros(num_samples, dtype=np.float32)
    for i in range(num_samples):
        b22 = (reg >> 22) & 1
        b17 = (reg >> 17) & 1
        feedback = b22 ^ b17
        reg = ((reg << 1) | feedback) & 0x7FFFFF
        val = ((reg >> 15) & 0xFF) / 127.5 - 1.0
        out[i] = val
    return out

def synth_pulse(freq, duty, num_samples, sample_rate, decay=None, loop_beg=None):
    t = np.arange(num_samples) / sample_rate
    phase = (t * freq) % 1.0
    wave = np.where(phase < duty, 0.85, -0.85).astype(np.float32)
    if decay is not None:
        if loop_beg is not None and loop_beg > 0:
            env = np.ones(num_samples, dtype=np.float32)
            env[:loop_beg] = np.exp(-t[:loop_beg] * decay)
            env[loop_beg:] = env[loop_beg - 1]
            wave *= env
        else:
            wave *= np.exp(-t * decay)
    return wave

def synth_triangle(freq, num_samples, sample_rate, decay=None, loop_beg=None):
    t = np.arange(num_samples) / sample_rate
    phase = (t * freq) % 1.0
    wave = (2.0 * np.abs(2.0 * phase - 1.0) - 1.0).astype(np.float32) * 0.9
    if decay is not None:
        if loop_beg is not None and loop_beg > 0:
            env = np.ones(num_samples, dtype=np.float32)
            env[:loop_beg] = np.exp(-t[:loop_beg] * decay)
            env[loop_beg:] = env[loop_beg - 1]
            wave *= env
        else:
            wave *= np.exp(-t * decay)
    return wave

def synth_sid_bass(freq, num_samples, sample_rate, decay=None, loop_beg=None):
    """Warm C64 SID filtered bass: triangle + filtered pulse with snappy attack."""
    t = np.arange(num_samples) / sample_rate
    phase = (t * freq) % 1.0
    tri = 2.0 * np.abs(2.0 * phase - 1.0) - 1.0
    pulse = np.where(phase < 0.4, 0.6, -0.6)
    # add slight warm 2nd harmonic
    harm2 = 0.25 * np.sin(4 * np.pi * phase)
    wave = (0.6 * tri + 0.3 * pulse + harm2).astype(np.float32) * 0.95
    if decay is not None:
        if loop_beg is not None and loop_beg > 0:
            env = np.ones(num_samples, dtype=np.float32)
            # punchy initial attack
            env[:loop_beg] = 0.5 + 0.5 * np.exp(-t[:loop_beg] * decay)
            env[loop_beg:] = env[loop_beg - 1]
            wave *= env
        else:
            wave *= np.exp(-t * decay)
    return wave

def synth_bell_chime(freq, num_samples, sample_rate, decay=None, loop_beg=None):
    """High sparkling C64 chime / celesta."""
    t = np.arange(num_samples) / sample_rate
    phase = (t * freq) % 1.0
    pulse = np.where(phase < 0.15, 0.7, -0.7)
    sine = np.sin(2 * np.pi * phase)
    sine2 = 0.3 * np.sin(4 * np.pi * phase)
    wave = (0.5 * pulse + 0.35 * sine + 0.15 * sine2).astype(np.float32) * 0.9
    if decay is not None:
        if loop_beg is not None and loop_beg > 0:
            env = np.ones(num_samples, dtype=np.float32)
            env[:loop_beg] = np.exp(-t[:loop_beg] * decay)
            env[loop_beg:] = env[loop_beg - 1]
            wave *= env
        else:
            wave *= np.exp(-t * decay)
    return wave

def synth_sid_kick(num_samples, sample_rate):
    """Authentic C64 SID kick drum: pitch drop + click."""
    t = np.arange(num_samples) / sample_rate
    freq = 200.0 * np.exp(-t * 45.0) + 40.0
    phase = 2 * np.pi * np.cumsum(freq) / sample_rate
    body = np.sin(phase) * np.exp(-t * 22.0)
    # transient click
    click = np.zeros(num_samples, dtype=np.float32)
    n_click = min(int(sample_rate * 0.005), num_samples)
    click[:n_click] = (np.random.rand(n_click) * 2.0 - 1.0) * np.exp(-t[:n_click] * 500.0)
    return (0.85 * body + 0.3 * click).astype(np.float32)

def synth_sid_snare(num_samples, sample_rate):
    """Authentic C64 SID noise snare drum with tone body."""
    t = np.arange(num_samples) / sample_rate
    # Tone body
    freq = 280.0 * np.exp(-t * 30.0) + 80.0
    phase = 2 * np.pi * np.cumsum(freq) / sample_rate
    body = np.sin(phase) * np.exp(-t * 25.0) * 0.4
    # Noise body
    noise = sid_lfsr_noise(num_samples) * np.exp(-t * 18.0) * 0.75
    return (body + noise).astype(np.float32)

def synth_sid_hihat(num_samples, sample_rate, is_open=False):
    """Crisp C64 SID hihat."""
    t = np.arange(num_samples) / sample_rate
    decay_rate = 12.0 if is_open else 65.0
    noise = sid_lfsr_noise(num_samples)
    # Highpass filter simulation: subtract moving average
    filtered_noise = np.zeros(num_samples, dtype=np.float32)
    for i in range(1, num_samples):
        filtered_noise[i] = noise[i] - 0.7 * noise[i - 1]
    return (filtered_noise * np.exp(-t * decay_rate) * 0.8).astype(np.float32)

def synth_sid_cymbal(num_samples, sample_rate):
    """C64 SID crash/ride cymbal."""
    t = np.arange(num_samples) / sample_rate
    noise = sid_lfsr_noise(num_samples)
    # Ring mod metallic tone
    f1, f2 = 540.0, 830.0
    mod = (np.sin(2 * np.pi * f1 * t) * np.sin(2 * np.pi * f2 * t)) * 0.3
    cymbal = (noise * 0.7 + mod) * np.exp(-t * 6.5) * 0.8
    return cymbal.astype(np.float32)

def synth_sid_tom(num_samples, sample_rate, pitch_factor=1.0):
    """C64 SID tom pitch sweep."""
    t = np.arange(num_samples) / sample_rate
    freq = (260.0 * pitch_factor) * np.exp(-t * 18.0) + (60.0 * pitch_factor)
    phase = 2 * np.pi * np.cumsum(freq) / sample_rate
    body = (0.7 * np.sin(phase) + 0.3 * np.sin(2 * phase)) * np.exp(-t * 14.0)
    noise = sid_lfsr_noise(num_samples) * np.exp(-t * 40.0) * 0.2
    return (body + noise).astype(np.float32)


# ==========================================
# 2. IT MODULE REBUILDER & SAMPLE REPLACER
# ==========================================

def rebuild_it_with_sid_samples(in_path, out_path, smp_classifier):
    """
    Parses in_path, replaces sample waveforms according to smp_classifier,
    and writes out a valid IT module.
    """
    with open(in_path, 'rb') as f:
        data = f.read()

    ordnum, insnum, smpnum, patnum = struct.unpack('<HHHH', data[0x20:0x28])
    header_end = 0x00C0
    orders = data[header_end : header_end + ordnum]

    offset = header_end + ordnum
    ins_offsets = list(struct.unpack(f'<{insnum}I', data[offset : offset + insnum * 4]))
    offset += insnum * 4
    smp_offsets = list(struct.unpack(f'<{smpnum}I', data[offset : offset + smpnum * 4]))
    offset += smpnum * 4
    pat_offsets = list(struct.unpack(f'<{patnum}I', data[offset : offset + patnum * 4]))

    ins_blocks = []
    for i in range(insnum):
        off = ins_offsets[i]
        next_off = ins_offsets[i + 1] if i + 1 < insnum else (smp_offsets[0] if smpnum > 0 else pat_offsets[0])
        ins_blocks.append(data[off : next_off])

    smp_headers = []
    new_smp_datas = []

    for i in range(smpnum):
        off = smp_offsets[i]
        hdr = bytearray(data[off : off + 0x50])
        length = struct.unpack('<I', hdr[0x30:0x34])[0]
        loop_beg = struct.unpack('<I', hdr[0x34:0x38])[0]
        loop_end = struct.unpack('<I', hdr[0x38:0x3C])[0]
        c5speed = struct.unpack('<I', hdr[0x3C:0x40])[0]
        smp_name = hdr[0x14:0x30].decode('latin1', errors='ignore').strip()
        dos_name = hdr[4:16].decode('latin1', errors='ignore').strip()
        flags = hdr[0x12]
        is_loop = bool(flags & 0x10)

        # Classify and synthesize
        new_wave = smp_classifier(i + 1, smp_name, dos_name, length, loop_beg, loop_end, c5speed, is_loop)
        
        # Enforce loop continuity
        if is_loop and loop_end > loop_beg:
            # Match endpoints at loop boundary
            loop_len = loop_end - loop_beg
            # Ensure seamless loop segment
            new_wave[loop_end - 1] = new_wave[loop_beg]

        # Convert to 16-bit signed PCM
        new_wave = np.clip(new_wave, -0.98, 0.98)
        pcm16 = (new_wave * 32767).astype(np.int16)

        # Force 16-bit uncompressed signed flag in header
        hdr[0x12] = (hdr[0x12] | 0x02) & (~0x08)  # Set 16-bit, clear compression
        hdr[0x2E] = 0x01  # Signed data

        smp_headers.append(hdr)
        new_smp_datas.append(pcm16.tobytes())

    pat_blocks = []
    for i in range(patnum):
        off = pat_offsets[i]
        pat_len = struct.unpack('<H', data[off : off + 2])[0]
        pat_blocks.append(data[off : off + 8 + pat_len])

    # Ensure seamless continuous looping: remove 255 stop markers and create endless order lists
    order_list = [x for x in orders if x != 255]
    if 'outdoors' in out_path.lower():
        order_list = order_list + [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 2]
    elif 'title' in out_path.lower():
        order_list = [0, 1, 0, 1, 0, 1, 0, 1]
    elif 'indoors' in out_path.lower():
        order_list = order_list + [0, 1, 2, 3, 4, 5, 6, 0]
    elif 'highscore' in out_path.lower():
        order_list = order_list + [0, 1, 0]

    ordnum = len(order_list)
    new_orders = bytes(order_list)

    # Build new file buffer
    hdr_bytes = bytearray(data[:header_end])
    struct.pack_into('<H', hdr_bytes, 0x20, ordnum)

    out = bytearray()
    out.extend(hdr_bytes)
    out.extend(new_orders)

    ins_table_off = len(out)
    out.extend(b'\x00' * (insnum * 4))
    smp_table_off = len(out)
    out.extend(b'\x00' * (smpnum * 4))
    pat_table_off = len(out)
    out.extend(b'\x00' * (patnum * 4))

    new_ins_offsets = []
    for b in ins_blocks:
        new_ins_offsets.append(len(out))
        out.extend(b)

    new_smp_offsets = []
    for h in smp_headers:
        new_smp_offsets.append(len(out))
        out.extend(h)

    new_pat_offsets = []
    for p in pat_blocks:
        new_pat_offsets.append(len(out))
        out.extend(p)

    for i in range(smpnum):
        smp_ptr = len(out)
        struct.pack_into('<I', smp_headers[i], 0x48, smp_ptr)
        hdr_off = new_smp_offsets[i]
        struct.pack_into('<I', out, hdr_off + 0x48, smp_ptr)
        out.extend(new_smp_datas[i])

    for i, off in enumerate(new_ins_offsets):
        struct.pack_into('<I', out, ins_table_off + i * 4, off)
    for i, off in enumerate(new_smp_offsets):
        struct.pack_into('<I', out, smp_table_off + i * 4, off)
    for i, off in enumerate(new_pat_offsets):
        struct.pack_into('<I', out, pat_table_off + i * 4, off)

    with open(out_path, 'wb') as f:
        f.write(out)
    print(f'Successfully generated SID IT track: {out_path} ({len(out)} bytes)')


# ==========================================
# 3. TRACK-SPECIFIC INSTRUMENT CLASSIFIERS
# ==========================================

def classify_title(idx, name, dos, length, loop_beg, loop_end, c5speed, is_loop):
    loop_len = (loop_end - loop_beg) if (is_loop and loop_end > loop_beg) else length
    f0 = (c5speed / loop_len) if loop_len > 0 else 261.63

    if idx == 6:  # Bass Drum
        return synth_sid_kick(length, c5speed)
    elif idx == 10:  # Snare
        return synth_sid_snare(length, c5speed)
    elif idx in (7, 9):  # Closed Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=False)
    elif idx == 8:  # Open Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=True)
    elif idx in (5, 11, 12):  # Cymbals
        return synth_sid_cymbal(length, c5speed)
    elif idx in (4, 21, 22, 23):  # Synth Bass 2 -> Punchy C64 SID Bass
        return synth_sid_bass(f0, length, c5speed, decay=4.0, loop_beg=loop_beg)
    elif idx in (1, 13, 14, 15, 16):  # Banjo -> Chris Huelsbeck SID Arpeggios
        return synth_pulse(f0, 0.25, length, c5speed, decay=8.0, loop_beg=loop_beg)
    elif idx in (2, 17, 18):  # Fantasia -> Soaring Chris Huelsbeck Lead Synth
        return synth_pulse(f0, 0.45, length, c5speed, decay=0.8, loop_beg=loop_beg)
    elif idx in (3, 19, 20):  # Celesta -> Sparkling High C64 Chimes
        return synth_bell_chime(f0, length, c5speed, decay=6.0, loop_beg=loop_beg)
    else:
        return synth_pulse(f0, 0.30, length, c5speed, decay=3.0, loop_beg=loop_beg)

def classify_outdoors(idx, name, dos, length, loop_beg, loop_end, c5speed, is_loop):
    loop_len = (loop_end - loop_beg) if (is_loop and loop_end > loop_beg) else length
    f0 = (c5speed / loop_len) if loop_len > 0 else 261.63

    if idx == 3:  # Bass Drum
        return synth_sid_kick(length, c5speed)
    elif idx == 13:  # Snare Drum
        return synth_sid_snare(length, c5speed)
    elif idx in (4, 6):  # Closed Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=False)
    elif idx == 5:  # Open Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=True)
    elif idx == 2:  # Crash
        return synth_sid_cymbal(length, c5speed)
    elif idx in (7, 8, 9, 10):  # Toms
        p_factors = {7: 1.2, 8: 1.0, 9: 0.85, 10: 0.7}
        return synth_sid_tom(length, c5speed, pitch_factor=p_factors.get(idx, 1.0))
    elif idx in (1, 15, 16, 17):  # Synth Bass 2 -> Classic C64 Walking Bass
        return synth_sid_bass(f0, length, c5speed, decay=8.0, loop_beg=loop_beg)
    elif idx in (11, 18, 19, 20, 21, 22):  # Square Wave -> Main Stage 1 Melody Lead
        return synth_pulse(f0, 0.25, length, c5speed, decay=3.0, loop_beg=loop_beg)
    elif idx in (12, 23, 24, 25, 26):  # Banjo -> SID Staccato Rhythm Plucks
        return synth_pulse(f0, 0.125, length, c5speed, decay=14.0, loop_beg=loop_beg)
    elif idx in (14, 27, 28):  # Celesta -> SID Bells
        return synth_bell_chime(f0, length, c5speed, decay=12.0, loop_beg=loop_beg)
    else:
        return synth_pulse(f0, 0.25, length, c5speed, decay=5.0, loop_beg=loop_beg)

def classify_indoors(idx, name, dos, length, loop_beg, loop_end, c5speed, is_loop):
    loop_len = (loop_end - loop_beg) if (is_loop and loop_end > loop_beg) else length
    f0 = (c5speed / loop_len) if loop_len > 0 else 261.63

    if idx in (5, 8, 9, 10, 11, 12):  # Fretless Bass -> Deep Cavern SID Bass
        return synth_sid_bass(f0, length, c5speed, decay=4.0, loop_beg=loop_beg)
    elif idx in (1, 7):  # Ice Rain -> Resonant SID Bell
        return synth_bell_chime(f0, length, c5speed, decay=5.0, loop_beg=loop_beg)
    elif idx == 2:  # Agogo -> Percussive Bell Tone
        return synth_bell_chime(f0, length, c5speed, decay=20.0, loop_beg=loop_beg)
    elif idx == 3:  # Seashore -> Resonant Cavern Wind (Filtered Noise)
        noise = sid_lfsr_noise(length)
        t = np.arange(length) / c5speed
        env = 0.5 + 0.5 * np.sin(2 * np.pi * 0.2 * t)
        return (noise * env * 0.6).astype(np.float32)
    elif idx == 4:  # Bird -> Chirping SID Pulse
        return synth_pulse(f0, 0.125, length, c5speed, decay=6.0, loop_beg=loop_beg)
    elif idx in (6, 13, 14, 15):  # Koto -> Snappy 25% Pulse Pluck
        return synth_pulse(f0, 0.25, length, c5speed, decay=12.0, loop_beg=loop_beg)
    else:
        return synth_pulse(f0, 0.25, length, c5speed, decay=5.0, loop_beg=loop_beg)

def classify_highscore(idx, name, dos, length, loop_beg, loop_end, c5speed, is_loop):
    loop_len = (loop_end - loop_beg) if (is_loop and loop_end > loop_beg) else length
    f0 = (c5speed / loop_len) if loop_len > 0 else 261.63

    if idx == 4:  # Bass Drum
        return synth_sid_kick(length, c5speed)
    elif idx == 8:  # Snare
        return synth_sid_snare(length, c5speed)
    elif idx == 10:  # Closed Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=False)
    elif idx == 9:  # Open Hi-Hat
        return synth_sid_hihat(length, c5speed, is_open=True)
    elif idx in (3, 7):  # Cymbals
        return synth_sid_cymbal(length, c5speed)
    elif idx in (2, 13, 14, 15):  # Synth Bass 2
        return synth_sid_bass(f0, length, c5speed, decay=6.0, loop_beg=loop_beg)
    elif idx in (1, 11, 12):  # Fantasia -> Soaring High Lead
        return synth_pulse(f0, 0.50, length, c5speed, decay=2.0, loop_beg=loop_beg)
    elif idx in (5, 16, 17, 18, 19):  # Banjo -> Fast Arpeggios
        return synth_pulse(f0, 0.25, length, c5speed, decay=14.0, loop_beg=loop_beg)
    elif idx in (6, 20, 21):  # Celesta -> Chimes
        return synth_bell_chime(f0, length, c5speed, decay=10.0, loop_beg=loop_beg)
    else:
        return synth_pulse(f0, 0.25, length, c5speed, decay=5.0, loop_beg=loop_beg)


# ==========================================
# 4. SOUND EFFECTS SYNTHESIS (.WAV)
# ==========================================

SR = 44100

def write_wav(path, data):
    data = np.clip(data, -0.95, 0.95)
    pcm16 = (data * 32767).astype(np.int16)
    with wave.open(path, 'wb') as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm16.tobytes())
    print(f'Wrote SID SFX: {path} ({len(data)/SR:.3f}s)')

def generate_sfx():
    # 1. JUMP: Classic C64 rising pulse chirp (220 Hz -> 680 Hz in 0.16s)
    num = int(SR * 0.16)
    t = np.arange(num) / SR
    freq = 220.0 + (680.0 - 220.0) * (t / 0.16)**0.85
    phase = 2 * np.pi * np.cumsum(freq) / SR
    pulse = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.3), 0.85, -0.85)
    env = np.exp(-t * 10.0)
    write_wav('base/audio/jump.wav', pulse * env)

    # 2. DING: Sparkling high-pitch two-tone diamond chime (1760 Hz -> 2637 Hz)
    num = int(SR * 0.11)
    t = np.arange(num) / SR
    split = int(SR * 0.035)
    f = np.zeros(num)
    f[:split] = 1760.0  # A6
    f[split:] = 2637.0  # E7
    phase = 2 * np.pi * np.cumsum(f) / SR
    wave_d = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.2), 0.85, -0.85)
    env = np.exp(-t * 28.0)
    write_wav('base/audio/ding.wav', wave_d * env)

    # 3. CRACK: Punchy head bump into block (low square + fast noise tick)
    num = int(SR * 0.08)
    t = np.arange(num) / SR
    freq = 140.0 * np.exp(-t * 30.0) + 60.0
    phase = 2 * np.pi * np.cumsum(freq) / SR
    sqr = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.5), 0.85, -0.85)
    noise = sid_lfsr_noise(num)
    n_env = np.exp(-t * 80.0)
    s_env = np.exp(-t * 40.0)
    write_wav('base/audio/crack.wav', 0.6 * sqr * s_env + 0.4 * noise * n_env)

    # 4. DISK: Crunchy C64 SID brick smash
    num = int(SR * 0.24)
    t = np.arange(num) / SR
    noise = sid_lfsr_noise(num)
    freq = 320.0 * np.exp(-t * 15.0) + 70.0
    phase = 2 * np.pi * np.cumsum(freq) / SR
    body = np.sin(phase) * np.exp(-t * 16.0)
    env = np.exp(-t * 14.0)
    write_wav('base/audio/disk.wav', (0.75 * noise + 0.35 * body) * env)

    # 5. STOMP: Rapid descending squish (550 Hz -> 90 Hz in 0.14s)
    num = int(SR * 0.14)
    t = np.arange(num) / SR
    freq = 550.0 * np.exp(-t * 22.0) + 80.0
    phase = 2 * np.pi * np.cumsum(freq) / SR
    pulse = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.25), 0.85, -0.85)
    noise = sid_lfsr_noise(num)
    env = np.exp(-t * 20.0)
    n_env = np.exp(-t * 60.0)
    write_wav('base/audio/stomp.wav', (0.7 * pulse + 0.3 * noise * n_env) * env)

    # 6. SHOOT: Fast laser/fireball zap (1400 Hz -> 240 Hz in 0.12s)
    num = int(SR * 0.12)
    t = np.arange(num) / SR
    freq = 1400.0 * np.exp(-t * 25.0) + 200.0
    phase = 2 * np.pi * np.cumsum(freq) / SR
    pulse = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.3), 0.85, -0.85)
    env = np.exp(-t * 18.0)
    write_wav('base/audio/shoot.wav', pulse * env)

    # 7. MORPHING: Iconic ascending arpeggio flourish + powerup fanfare
    arpeggio_notes = [
        261.63, 329.63, 392.00, 523.25, 659.25, 783.99, 1046.50, 1318.51, 1567.98, 2093.00
    ]
    step_samples = int(SR * 0.035)  # 35ms per note
    total_arp_samples = step_samples * len(arpeggio_notes)
    sustain_samples = int(SR * 0.30)
    total_samples = total_arp_samples + sustain_samples
    
    t_tot = np.arange(total_samples) / SR
    wave_m = np.zeros(total_samples, dtype=np.float32)
    
    # Arpeggio phase
    curr_phase = 0.0
    for idx, f in enumerate(arpeggio_notes):
        start = idx * step_samples
        end = start + step_samples
        dur = end - start
        t_sub = np.arange(dur) / SR
        ph = 2 * np.pi * f * t_sub + curr_phase
        curr_phase = ph[-1] % (2 * np.pi)
        p = np.where((ph % (2 * np.pi)) < (2 * np.pi * 0.25), 0.85, -0.85)
        wave_m[start:end] = p
        
    # Sustain trill at top C7/G6 with vibrato
    t_sus = np.arange(sustain_samples) / SR
    vib = 2093.00 * (1.0 + 0.03 * np.sin(2 * np.pi * 25.0 * t_sus))
    ph_sus = 2 * np.pi * np.cumsum(vib) / SR + curr_phase
    p_sus = np.where((ph_sus % (2 * np.pi)) < (2 * np.pi * 0.25), 0.85, -0.85)
    env_sus = np.exp(-t_sus * 6.0)
    wave_m[total_arp_samples:] = p_sus * env_sus
    
    write_wav('base/audio/morphing.wav', wave_m)

    # 8. DIED: Mournful C64 death fanfare / descending slide
    num = int(SR * 1.4)
    t = np.arange(num) / SR
    # Descending chromatic notes (G4 down to C4)
    notes = [392.0, 369.99, 349.23, 329.63, 311.13, 293.66, 277.18, 261.63]
    dur_per_note = int(SR * 0.15)
    wave_died = np.zeros(num, dtype=np.float32)
    
    # Initial death crunch
    n_crunch = int(SR * 0.1)
    wave_died[:n_crunch] += sid_lfsr_noise(n_crunch) * np.exp(-t[:n_crunch] * 30.0) * 0.8
    
    ph = 0.0
    for i, n_freq in enumerate(notes):
        start = int(SR * 0.08) + i * dur_per_note
        end = min(start + dur_per_note, num)
        if start >= num:
            break
        t_sub = np.arange(end - start) / SR
        p_sub = 2 * np.pi * n_freq * t_sub + ph
        ph = p_sub[-1] % (2 * np.pi)
        pulse = np.where((p_sub % (2 * np.pi)) < (2 * np.pi * 0.35), 0.8, -0.8)
        env = np.exp(-t_sub * 4.0)
        wave_died[start:end] += pulse * env
        
    write_wav('base/audio/died.wav', wave_died)

    # 9. CLICK: Snappy 880 Hz retro UI blip
    num = int(SR * 0.025)
    t = np.arange(num) / SR
    phase = 2 * np.pi * 880.0 * t
    pulse = np.where((phase % (2 * np.pi)) < (2 * np.pi * 0.5), 0.8, -0.8)
    env = np.exp(-t * 90.0)
    write_wav('base/audio/click.wav', pulse * env)

    # 10. PORTAL: Swirling resonant PWM pulse warp
    num = int(SR * 0.35)
    t = np.arange(num) / SR
    freq = 300.0 + 500.0 * np.sin(2 * np.pi * 7.0 * t)
    phase = 2 * np.pi * np.cumsum(freq) / SR
    duty = 0.5 + 0.35 * np.sin(2 * np.pi * 15.0 * t)
    pulse = np.where((phase % (2 * np.pi)) < (2 * np.pi * duty), 0.85, -0.85)
    env = np.sin(np.pi * t / 0.35)
    write_wav('base/audio/portal.wav', pulse * env)


# ==========================================
# 5. MAIN EXECUTION
# ==========================================

if __name__ == '__main__':
    print('--- Synthesizing Authentic C64 MOS 6581 Sound Effects ---')
    generate_sfx()

    print('\n--- Generating Authentic C64 SID Impulse Tracker Tracks ---')
    rebuild_it_with_sid_samples('base/music/title.it', 'base/music/title.it', classify_title)
    rebuild_it_with_sid_samples('base/music/outdoors.it', 'base/music/outdoors.it', classify_outdoors)
    rebuild_it_with_sid_samples('base/music/indoors.it', 'base/music/indoors.it', classify_indoors)
    rebuild_it_with_sid_samples('base/music/highscore.it', 'base/music/highscore.it', classify_highscore)

    print('\n=== All SID Audio Generation Completed Successfully! ===')
