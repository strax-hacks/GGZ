#!/usr/bin/env python3
"""
Generate pixel-perfect Commodore 64 Font and Giana Brick UI assets.
"""
from PIL import Image, ImageDraw

def generate_c64_font():
    # 8x8 C64 PETSCII / Arcade Font Bitmaps
    # Each character is defined as 8 bytes (each row 8 bits, 1 = pixel, 0 = blank)
    font_bitmaps = {
        ' ': [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
        '!': [0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00],
        '"': [0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00],
        '#': [0x66, 0x66, 0xFF, 0x66, 0xFF, 0x66, 0x66, 0x00],
        '$': [0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x18, 0x00],
        '%': [0x62, 0x66, 0x0C, 0x18, 0x30, 0x66, 0x46, 0x00],
        '&': [0x3C, 0x66, 0x3C, 0x38, 0x67, 0x66, 0x3F, 0x00],
        '\'': [0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00],
        '(': [0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00],
        ')': [0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00],
        '*': [0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00],
        '+': [0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00],
        ',': [0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30],
        '-': [0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00],
        '.': [0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00],
        '/': [0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00],
        '0': [0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00],
        '1': [0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00],
        '2': [0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x7E, 0x00],
        '3': [0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00],
        '4': [0x0C, 0x1C, 0x34, 0x64, 0x7E, 0x04, 0x04, 0x00],
        '5': [0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00],
        '6': [0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00],
        '7': [0x7E, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x00],
        '8': [0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00],
        '9': [0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00],
        ':': [0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00],
        ';': [0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30, 0x00],
        '<': [0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00],
        '=': [0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00],
        '>': [0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00],
        '?': [0x3C, 0x66, 0x06, 0x0C, 0x18, 0x00, 0x18, 0x00],
        '@': [0x3C, 0x66, 0x6E, 0x6E, 0x60, 0x62, 0x3C, 0x00],
        'A': [0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00],
        'B': [0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00],
        'C': [0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00],
        'D': [0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00],
        'E': [0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00],
        'F': [0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00],
        'G': [0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3A, 0x00],
        'H': [0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00],
        'I': [0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00],
        'J': [0x0E, 0x06, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00],
        'K': [0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00],
        'L': [0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00],
        'M': [0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00],
        'N': [0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x00],
        'O': [0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'P': [0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00],
        'Q': [0x3C, 0x66, 0x66, 0x66, 0x6A, 0x6C, 0x36, 0x00],
        'R': [0x7C, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x63, 0x00],
        'S': [0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00],
        'T': [0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00],
        'U': [0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'V': [0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00],
        'W': [0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00],
        'X': [0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00],
        'Y': [0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00],
        'Z': [0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00],
        '[': [0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00],
        '\\': [0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00],
        ']': [0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00],
        '^': [0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00],
        '_': [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00],
        'a': [0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00],
        'b': [0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x00],
        'c': [0x00, 0x00, 0x3C, 0x66, 0x60, 0x66, 0x3C, 0x00],
        'd': [0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00],
        'e': [0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00],
        'f': [0x1C, 0x30, 0x78, 0x30, 0x30, 0x30, 0x30, 0x00],
        'g': [0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x7C],
        'h': [0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00],
        'i': [0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00],
        'j': [0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x6C, 0x38],
        'k': [0x60, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x00],
        'l': [0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00],
        'm': [0x00, 0x00, 0x76, 0x7F, 0x6B, 0x63, 0x63, 0x00],
        'n': [0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00],
        'o': [0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'p': [0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60],
        'q': [0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x07],
        'r': [0x00, 0x00, 0x7C, 0x66, 0x60, 0x60, 0x60, 0x00],
        's': [0x00, 0x00, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x00],
        't': [0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x0E, 0x00],
        'u': [0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00],
        'v': [0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00],
        'w': [0x00, 0x00, 0x63, 0x6B, 0x7F, 0x77, 0x36, 0x00],
        'x': [0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00],
        'y': [0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x7C],
        'z': [0x00, 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x7E, 0x00],
        'Ä': [0x66, 0x00, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00],
        'Ö': [0x66, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'Ü': [0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'ä': [0x00, 0x66, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00],
        'ö': [0x00, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00],
        'ü': [0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00],
        'ß': [0x3C, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x00],
        '◆': [0x18, 0x3C, 0x7E, 0xFF, 0x7E, 0x3C, 0x18, 0x00],
        '★': [0x18, 0x18, 0x7E, 0x3C, 0x7E, 0x66, 0x66, 0x00],
        '♥': [0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00],
    }

    # Load existing Font.png to preserve right side (the 16x16 big font at x >= 320)
    orig_font = Image.open('base/Font.png').convert('RGBA')
    new_font = Image.new('RGBA', (800, 160), (0, 0, 0, 0))
    new_font.paste(orig_font.crop((320, 0, 800, 160)), (320, 0))

    # C64 color sets:
    # 0: Pure White (#FFFFFF)
    # 1: C64 Light Blue (#8888FF)
    # 2: C64 Gold / Yellow (#FFEE77)
    # 3: C64 Light Green (#89E581 / 137, 229, 129)
    colors = [
        (255, 255, 255, 255),  # 0: White
        (136, 136, 255, 255),  # 1: Light Blue
        (255, 238, 119, 255),  # 2: Gold / Yellow
        (137, 229, 129, 255),  # 3: Authentic C64 Light Green
    ]

    def draw_glyph_8x8(dest_img, char, base_x, base_y, color):
        bmp = font_bitmaps.get(char, font_bitmaps[' '])
        for row in range(8):
            byte = bmp[row]
            for col in range(8):
                if (byte >> (7 - col)) & 1:
                    dest_img.putpixel((base_x + col, base_y + row), color)

    for c_idx, color in enumerate(colors):
        y_offset = c_idx * 32

        # Row 0 (y=0..7): Uppercase A-Z
        for i, ch in enumerate("ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
            draw_glyph_8x8(new_font, ch, i * 8, y_offset + 0, color)

        # Row 1 (y=8..15): Lowercase a-z
        for i, ch in enumerate("abcdefghijklmnopqrstuvwxyz"):
            draw_glyph_8x8(new_font, ch, i * 8, y_offset + 8, color)

        # Row 2 (y=16..23): Space + 0-9 + umlauts + symbols
        draw_glyph_8x8(new_font, ' ', 0, y_offset + 16, color)
        for i, ch in enumerate("0123456789"):
            draw_glyph_8x8(new_font, ch, (i + 1) * 8, y_offset + 16, color)
        for i, ch in enumerate(['Ä', 'Ö', 'Ü', 'ä', 'ö', 'ü', 'ß', '◆', '★', '♥']):
            draw_glyph_8x8(new_font, ch, (11 + i) * 8, y_offset + 16, color)

        # Row 3 (y=24..31): Punctuation
        punct = ['.', ',', '?', '!', '\'', '-', '+', '*', '/', '"', '(', ')', '=', ':', ';', '<', '>', '@', '#', '$', '%', '&', '_', '[', ']', '\\', '^']
        for i, ch in enumerate(punct):
            draw_glyph_8x8(new_font, ch, i * 8, y_offset + 24, color)

    new_font.save('base/Font.png')
    print('Generated authentic 8x8 C64 base/Font.png')

def generate_giana_brick_interface():
    iface = Image.open('base/Interface.png').convert('RGBA')
    w, h = iface.size

    # Clear region y=208..340, x=0..640 (remove old PC grey cards & clipart icons)
    draw = ImageDraw.Draw(iface)
    draw.rectangle([0, 208, 639, 340], fill=(0, 0, 0, 0))

    def create_giana_brick_card(bw, bh, active=False):
        card = Image.new('RGBA', (bw, bh), (0, 0, 0, 0))

        if not active:
            # Authentic C64 Giana Brown Brick Colors
            c_mortar   = (0, 0, 0, 255)         # Black mortar
            c_bevel_hi = (184, 122, 56, 255)    # Top/left bevel highlight
            c_bevel_mid= (144, 95, 37, 255)     # Inner bevel
            c_bevel_sh = (0, 0, 0, 255)         # Bottom/right shadow
            c_brick_hi = (168, 112, 44, 255)    # Top edge of brick
            c_brick_mid= (144, 95, 37, 255)     # Brick body
            c_brick_sh = (92, 71, 0, 255)       # Bottom edge of brick
            c_rivet    = (208, 220, 113, 255)   # Corner rivet
        else:
            # Active Glowing Golden Brick Colors
            c_mortar   = (20, 10, 0, 255)       # Deep shadow mortar
            c_bevel_hi = (255, 255, 220, 255)   # Gleaming white-yellow top/left
            c_bevel_mid= (255, 215, 50, 255)    # Radiant gold inner
            c_bevel_sh = (120, 60, 10, 255)     # Shadow
            c_brick_hi = (255, 180, 50, 255)    # Highlighted golden brick top
            c_brick_mid= (180, 95, 25, 255)     # Rich glowing amber body (high contrast for white text)
            c_brick_sh = (130, 65, 15, 255)     # Golden brick bottom
            c_rivet    = (255, 255, 240, 255)   # Radiant corner sparkle

        course_height = 6  # 5px brick + 1px mortar
        brick_width = 20   # 19px brick + 1px mortar

        for y in range(bh):
            course = y // course_height
            y_in_course = y % course_height
            is_mortar_h = (y_in_course == course_height - 1)

            stagger = (brick_width // 2) if (course % 2 == 1) else 0

            for x in range(bw):
                is_mortar_v = ((x + stagger) % brick_width == brick_width - 1)

                if is_mortar_h or is_mortar_v:
                    card.putpixel((x, y), c_mortar)
                else:
                    if y_in_course == 0:
                        card.putpixel((x, y), c_brick_hi)
                    elif y_in_course == course_height - 2:
                        card.putpixel((x, y), c_brick_sh)
                    else:
                        card.putpixel((x, y), c_brick_mid)

        # Beveled Outer Border (2px)
        # Top 2 rows
        for x in range(bw):
            card.putpixel((x, 0), c_bevel_hi)
            card.putpixel((x, 1), c_bevel_mid)
        # Left 2 cols
        for y in range(bh):
            card.putpixel((0, y), c_bevel_hi)
            card.putpixel((1, y), c_bevel_mid)
        # Bottom 2 rows
        for x in range(bw):
            card.putpixel((x, bh - 1), c_bevel_sh)
            card.putpixel((x, bh - 2), c_bevel_sh)
        # Right 2 cols
        for y in range(bh):
            card.putpixel((bw - 1, y), c_bevel_sh)
            card.putpixel((bw - 2, y), c_bevel_sh)

        # 2x2 Corner Rivets
        for cx, cy in [(2, 2), (bw - 4, 2), (2, bh - 4), (bw - 4, bh - 4)]:
            for dx in range(2):
                for dy in range(2):
                    card.putpixel((cx + dx, cy + dy), c_rivet)

        return card

    # Generate 128x32 card at (0, 208) [normal] and (128, 208) [active]
    btn_norm = create_giana_brick_card(128, 32, active=False)
    btn_act  = create_giana_brick_card(128, 32, active=True)
    iface.paste(btn_norm, (0, 208), btn_norm)
    iface.paste(btn_act, (128, 208), btn_act)

    # Also generate taller 128x64 versions at y=240
    btn_norm_tall = create_giana_brick_card(128, 64, active=False)
    btn_act_tall  = create_giana_brick_card(128, 64, active=True)
    iface.paste(btn_norm_tall, (0, 240), btn_norm_tall)
    iface.paste(btn_act_tall, (128, 240), btn_act_tall)

    iface.save('base/Interface.png')
    print('Generated Giana Brick buttons in base/Interface.png')

def generate_c64_intro_tiles():
    tiles_img = Image.open('base/c64/Tiles.png').convert('RGBA')

    C_BLACK   = (0, 0, 0, 255)
    C_LGREY   = (182, 182, 182, 255)
    C_MGREY   = (129, 129, 129, 255)
    C_DGREY   = (85, 85, 85, 255)
    C_WHITE   = (255, 255, 255, 255)
    C_GREEN   = (137, 229, 129, 255)
    C_RED     = (187, 106, 81, 255)
    C_YELLOW  = (255, 255, 123, 255)
    C_BLUE    = (105, 83, 245, 255)
    C_DBLUE   = (65, 55, 205, 255)
    C_CYAN    = (170, 255, 238, 255)
    C_TRANS   = (255, 0, 255, 255)

    def create_cobble_top():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            for x in range(16):
                if y < 8:
                    cx = x % 8
                    if y == 0 and (cx in (0, 7)):
                        img.putpixel((x, y), C_TRANS)
                    elif cx == 0 or cx == 7 or y == 7:
                        img.putpixel((x, y), C_BLACK)
                    elif y <= 2 or cx == 1:
                        img.putpixel((x, y), C_LGREY)
                    else:
                        img.putpixel((x, y), C_MGREY)
                else:
                    cx = (x + 4) % 8
                    if cx == 0 or cx == 7 or y == 15:
                        img.putpixel((x, y), C_BLACK)
                    elif y <= 9 or cx == 1:
                        img.putpixel((x, y), C_LGREY)
                    else:
                        img.putpixel((x, y), C_MGREY)
        return img

    def create_cobble_bot():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            for x in range(16):
                if y < 8:
                    cx = x % 8
                    if cx == 0 or cx == 7 or y == 7:
                        img.putpixel((x, y), C_BLACK)
                    elif y <= 1 or cx == 1:
                        img.putpixel((x, y), C_LGREY)
                    else:
                        img.putpixel((x, y), C_MGREY)
                else:
                    cx = (x + 4) % 8
                    if cx == 0 or cx == 7 or y == 15:
                        img.putpixel((x, y), C_BLACK)
                    elif y <= 9 or cx == 1:
                        img.putpixel((x, y), C_LGREY)
                    else:
                        img.putpixel((x, y), C_MGREY)
        return img

    def create_brick_wall():
        img = Image.new('RGBA', (16, 16), C_BLACK)
        for y in range(16):
            course = y // 4
            y_in = y % 4
            stagger = 8 if (course % 2 == 1) else 0
            for x in range(16):
                is_mortar_h = (y_in == 3)
                is_mortar_v = ((x + stagger) % 8 == 7)
                if is_mortar_h or is_mortar_v:
                    img.putpixel((x, y), C_BLACK)
                elif y_in == 0:
                    img.putpixel((x, y), C_LGREY)
                else:
                    img.putpixel((x, y), C_MGREY)
        return img

    def create_hatched_pipe():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            img.putpixel((0, y), C_BLACK)
            img.putpixel((15, y), C_BLACK)
            for x in range(1, 15):
                phase = (x + y) % 4
                if phase == 0:
                    img.putpixel((x, y), C_WHITE)
                elif phase == 1:
                    img.putpixel((x, y), C_LGREY)
                elif phase == 2:
                    img.putpixel((x, y), C_MGREY)
                else:
                    img.putpixel((x, y), C_BLACK)
        return img

    def create_hatched_cap():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            for x in range(16):
                if y < 4:
                    if 6 <= x <= 9:
                        img.putpixel((x, y), C_LGREY if y < 2 else C_MGREY)
                elif y < 8:
                    if 4 <= x <= 11:
                        img.putpixel((x, y), C_LGREY if y < 6 else C_MGREY)
                else:
                    if 2 <= x <= 13:
                        if x == 2 or x == 13 or y == 15:
                            img.putpixel((x, y), C_BLACK)
                        elif y == 8:
                            img.putpixel((x, y), C_WHITE)
                        elif y == 9:
                            img.putpixel((x, y), C_LGREY)
                        else:
                            img.putpixel((x, y), C_MGREY)
        return img

    def create_water_in_g():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            for x in range(16):
                if y < 3:
                    if (x % 4 in (0, 1) and y == 1) or (x % 4 in (2, 3) and y == 2):
                        img.putpixel((x, y), C_WHITE)
                    elif y >= 2:
                        img.putpixel((x, y), C_BLUE)
                elif y < 14:
                    img.putpixel((x, y), C_BLUE)
                else:
                    img.putpixel((x, y), C_BLACK)
        return img

    def create_mushroom_cap_and_stem():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        for y in range(16):
            for x in range(16):
                if 2 <= y <= 9:
                    dx = abs(x - 7.5)
                    dy = (y - 9)
                    if dx*dx + dy*dy * 1.4 <= 38:
                        if (x in (3, 4) and y in (4, 5)) or (x in (11, 12) and y in (4, 5)) or (x in (7, 8) and y in (5, 6)) or (x in (5, 6) and y in (3, 4)) or (x in (9, 10) and y in (3, 4)):
                            img.putpixel((x, y), C_WHITE)
                        elif y == 2 or dx > 5.5:
                            img.putpixel((x, y), C_BLACK)
                        else:
                            img.putpixel((x, y), C_RED)
                elif 10 <= y <= 15:
                    if 5 <= x <= 10:
                        if x == 5 or x == 10 or y == 15:
                            img.putpixel((x, y), C_BLACK)
                        elif (x + y) % 2 == 0:
                            img.putpixel((x, y), C_YELLOW)
                        else:
                            img.putpixel((x, y), C_WHITE)
        return img

    def create_bush_pair():
        bush_art = [
            '................................',
            '............##.###..............',
            '.........#######.######.........',
            '.......##################.......',
            '.....######################.....',
            '....########################....',
            '...##########################...',
            '..############################..',
            '..############################..',
            '.##############################.',
            '.##############################.',
            '.#############..###############.',
            '.############.MM.##############.',
            '..##########.MLLM.############..',
            '...#########.MMMM.###########...',
            '.....######..MMMM..########.....',
        ]
        bush = Image.new('RGBA', (32, 16), C_TRANS)
        for y in range(16):
            row = bush_art[y]
            for x in range(32):
                c = row[x]
                if c == '#':
                    bush.putpixel((x, y), C_GREEN)
                elif c == 'M':
                    bush.putpixel((x, y), C_MGREY)
                elif c == 'L':
                    bush.putpixel((x, y), C_LGREY)
        left = bush.crop((0, 0, 16, 16))
        right = bush.crop((16, 0, 32, 16))
        return left, right

    def create_full_bevel_block():
        img = Image.new('RGBA', (16, 16), C_BLACK)
        pattern = [
            '.###############',
            '.###############',
            '.##.............',
            '.##.............',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##..==========#',
            '.##............#',
            '................',
        ]
        for y in range(16):
            row = pattern[y]
            for x in range(16):
                c = row[x]
                if c == '#':
                    img.putpixel((x, y), C_LGREY)
                elif c == '=':
                    img.putpixel((x, y), C_MGREY)
                else:
                    img.putpixel((x, y), C_BLACK)
        return img

    def create_full_diamond():
        img = Image.new('RGBA', (16, 16), C_TRANS)
        art = [
            '................',
            '................',
            '.....######.....',
            '...##WWWWCC##...',
            '..#WWWWWWCCCC#..',
            '..#WWWWWWCCCC#..',
            '..#WWWWBBCCCC#..',
            '...#WWWBBCCD#...',
            '...#WWBBBCCD#...',
            '....#BBBDDD#....',
            '....#BBBDDD#....',
            '.....#BBDD#.....',
            '......####......',
            '................',
            '................',
            '................',
        ]
        for y in range(16):
            row = art[y]
            for x in range(16):
                c = row[x]
                if c == '#':
                    img.putpixel((x, y), C_BLACK)
                elif c == 'W':
                    img.putpixel((x, y), C_WHITE)
                elif c == 'C':
                    img.putpixel((x, y), C_CYAN)
                elif c == 'B':
                    img.putpixel((x, y), C_BLUE)
                elif c == 'D':
                    img.putpixel((x, y), C_DBLUE)
        return img

    def create_blue_water_surf(frame=0):
        img = Image.new('RGBA', (16, 16), C_DBLUE)
        shift = frame * 3
        for y in range(16):
            for x in range(16):
                if y < 4:
                    px = (x + shift) % 6
                    if (px < 3 and y == 1) or (px >= 3 and y == 2):
                        img.putpixel((x, y), C_WHITE)
                    elif y < 2:
                        img.putpixel((x, y), C_TRANS)
                    else:
                        img.putpixel((x, y), C_BLUE)
                elif y % 4 == 0:
                    img.putpixel((x, y), C_BLUE)
                else:
                    img.putpixel((x, y), C_DBLUE)
        return img

    def create_blue_water_deep():
        img = Image.new('RGBA', (16, 16), C_DBLUE)
        for y in range(16):
            for x in range(16):
                if y % 4 == 0:
                    img.putpixel((x, y), C_BLUE)
                else:
                    img.putpixel((x, y), C_DBLUE)
        return img

    tile_map = {
        2170: create_cobble_top(),
        2171: create_cobble_bot(),
        2172: create_brick_wall(),
        2173: create_hatched_pipe(),
        2174: create_hatched_cap(),
        2175: create_water_in_g(),
        2176: create_mushroom_cap_and_stem(),
        2179: create_full_bevel_block(),
        2180: create_blue_water_surf(0),
        2181: create_blue_water_deep(),
        2182: create_full_diamond(),
    }
    t_bl, t_br = create_bush_pair()
    tile_map[2177] = t_bl
    tile_map[2178] = t_br

    # Also update animated water frames at row 54 (2160..2168) and static water at row 6
    t_ws0 = create_blue_water_surf(0)
    t_ws1 = create_blue_water_surf(1)
    t_ws2 = create_blue_water_surf(2)
    t_wd  = create_blue_water_deep()

    tiles_img.paste(t_ws0, (0*16, 54*16))
    tiles_img.paste(t_ws0, (1*16, 54*16))
    tiles_img.paste(t_ws0, (2*16, 54*16))
    tiles_img.paste(t_ws1, (3*16, 54*16))
    tiles_img.paste(t_ws1, (4*16, 54*16))
    tiles_img.paste(t_ws1, (5*16, 54*16))
    tiles_img.paste(t_ws2, (6*16, 54*16))
    tiles_img.paste(t_ws2, (7*16, 54*16))
    tiles_img.paste(t_ws2, (8*16, 54*16))

    tiles_img.paste(t_ws0, (10*16, 6*16))
    tiles_img.paste(t_ws0, (11*16, 6*16))
    tiles_img.paste(t_ws0, (12*16, 6*16))
    tiles_img.paste(t_wd,  (16*16, 6*16))

    for tid, t_img in tile_map.items():
        c = tid % 40
        r = tid // 40
        tiles_img.paste(t_img, (c*16, r*16))

    tiles_img.save('base/c64/Tiles.png')
    print('Generated C64 authentic intro tiles in base/c64/Tiles.png')

if __name__ == '__main__':
    generate_c64_font()
    generate_giana_brick_interface()
    generate_c64_intro_tiles()
