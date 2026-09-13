# Rendering performance baseline

The headless `CaptureScreens` test uses SDL's deterministic dummy renderer and
compares the final 320x170 RGBA framebuffer with FNV-1a 64-bit hashes. These
hashes are the pixel-output contract for rendering optimizations.

| Scenario | Framebuffer hash | Scene submissions |
| --- | ---: | ---: |
| Stage 1, C64 Scaled | `0x58cfe7782eb8d8df` | 1403 |
| Stage 1, 1X Zoomed | `0x7b7b380768906955` | 615 |
| Stage 2, C64 Scaled | `0x89d159ec3762ac70` | 2093 |
| Stage 2, 1X Zoomed | `0x92234700f53f9d1e` | 714 |
| Stage 1, partially clipped entity and animated tile | `0x8b0df8b324f5f814` | 1397 |

The 60-frame C64 Scaled benchmark currently averages approximately 1379 scene
submissions per frame. Frame time is machine-dependent and is reported by the
test rather than stored as a fixed threshold.

After the pixel-preserving render-path optimization, the same scenarios produce
501, 384, 1191, 483, and 495 scene submissions respectively, and the 60-frame
C64 Scaled benchmark averages 477 submissions per frame. All framebuffer hashes
remain unchanged.

Tile animation now tracks only active cells. The regression benchmark verifies a
single active cell through two chained frames and confirms that the active list
returns to zero after the animation completes; no full 256x30 scan is used.

Submission counters are compiled into test targets only with
`OPENGGS_RENDER_STATS=1`; production builds do not pay the counter cost. The
hashes are backend-specific: hardware validation must use the same visual
comparison on the target renderer before accepting a backend-specific change.
