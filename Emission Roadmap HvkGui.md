\# Native HDR Emissive + Bloom for Dear ImGui (2D) — DX11 + DX12 Roadmap



Goal: Unreal-like emissive/glow for a pure 2D ImGui scene by rendering ImGui into HDR, extracting emissive, running a bloom chain, then compositing and presenting.

Targets: DX11 and DX12 backends with the same visual output and near-identical tuning.



---



\## 0) Non-negotiables (what “real emissive” needs)

\- UI must render into \*\*linear HDR\*\* (not straight to an LDR swapchain).

\- Emissive must be \*\*> 1.0\*\* (not faked by bright 8-bit colors).

\- Glow comes from a \*\*post-process bloom pipeline\*\* (threshold + mip chain + blur + upsample).

\- Final image is \*\*tone-mapped\*\* to the swapchain.



---



\## 1) Decide the common rendering contract (DX11/DX12 must match)

\### 1.1 Working color space

\- Author ImGui colors in sRGB-like UI space (same as now).

\- Convert to \*\*linear\*\* in the pixel shader (or use sRGB formats correctly).

\- All bloom math happens in \*\*linear HDR\*\*.



\### 1.2 Render targets and formats (recommended)

Create these logical textures (same names for both backends):

\- `UI\_HDR\_BASE`     : RGBA16F (linear)

\- `UI\_HDR\_EMISSIVE` : RGBA16F (linear)

\- `BLOOM\_MIP\[i]`    : RGBA16F (or R11G11B10F if you accept some tradeoffs)

\- `UI\_HDR\_COMPOSED` : RGBA16F (optional; can composite directly to backbuffer path)

\- `BACKBUFFER`      : swapchain format (DXGI\_FORMAT\_R8G8B8A8\_UNORM or \_SRGB)



Notes:

\- Use RGBA16F for least pain.

\- Keep alpha meaningful only in BASE. Emissive/bloom can ignore alpha.



\### 1.3 Blending model

Pick one and use it everywhere:

\- Recommended: \*\*premultiplied alpha\*\* for UI base (clean edges with bloom).

\- If staying with straight alpha, expect edge halos and spend time fixing them.



---



\## 2) Break ImGui out of the backbuffer (offscreen UI pass)

\### 2.1 Replace “render to swapchain” with “render to UI\_HDR\_BASE”

\- DX11: set RTV = `UI\_HDR\_BASE`, clear to 0.

\- DX12: transition `UI\_HDR\_BASE` to RTV, set RTV, clear.



\### 2.2 Render ImGui draw lists as usual

\- Keep existing ImGui draw data traversal.

\- Use a dedicated ImGui UI pipeline state (PSO) that outputs linear HDR.



Deliverable:

\- You can render the whole ImGui UI into `UI\_HDR\_BASE` and present it (with a simple tone map) looking identical to old LDR output.



---



\## 3) Add emissive as a first-class concept (core ImGui changes)

You need emissive to be controllable per element and exceed 1.0.



\### 3.1 Choose data path (recommended: vertex + optional drawcmd)

\*\*A) Extend ImGui vertex format (best)\*\*

\- Fork ImGui.

\- Extend `ImDrawVert` to include:

&nbsp; - `float Emissive;` (strength), and optionally

&nbsp; - `ImU32 EmissiveColor;` or `float3 EmissiveRGB;` (tint)

\- Update all backends to match the new input layout.



\*\*B) Add per-draw command material params (useful for grouping)\*\*

\- Extend `ImDrawCmd` to include an index into a small “UI material table”:

&nbsp; - `emissiveStrength`, `emissiveTint`, `bloomGroup`, etc.



Recommended approach:

\- Vertex emissive = “this pixel glows”

\- Drawcmd params = “global multiplier / tint / grouping”



\### 3.2 Add authoring API (small, practical)

Add helpers (your fork or your own wrapper layer):

\- `PushEmissive(float strength, ImVec4 tint = white)` / `PopEmissive()`

\- `EmissiveText(strength, tint, "...")`

\- `EmissiveImage(...)` / `EmissiveButton(...)`



Deliverable:

\- You can tag UI elements as emissive with strength > 1.0.



---



\## 4) Output base + emissive in one pass (MRT) or two passes

\### Option A: MRT (recommended)

Render ImGui once, output:

\- RT0: `UI\_HDR\_BASE`

\- RT1: `UI\_HDR\_EMISSIVE`



Pixel shader:

\- Compute base color (linear) and write to RT0 (normal alpha blend).

\- Compute emissive contribution and write to RT1 (additive or overwrite).

&nbsp; - Emissive should be mostly black except glowing pixels.



DX11:

\- Set OMRenderTargets to 2 RTVs.

\- Create blend states that make sense per RT (DX11 blending can be set per target).



DX12:

\- PSO with 2 RTV formats.

\- Blend is defined in PSO per render target.



\### Option B: Two passes (simpler to debug)

\- Pass 1 renders BASE into `UI\_HDR\_BASE`.

\- Pass 2 renders only emissive-only geometry into `UI\_HDR\_EMISSIVE`.

This requires drawlist filtering or a second drawlist stream.



Deliverable:

\- `UI\_HDR\_EMISSIVE` contains only glow sources, not the full UI.



---



\## 5) Bloom pipeline (Unreal-like glow, for UI emissive only)

\### 5.1 Prefilter / threshold (soft knee)

Input: `UI\_HDR\_EMISSIVE`

Output: `BLOOM\_MIP\[0]` at 1/2 res (or full res if you want max quality)



Implement:

\- Luma-based or max-channel threshold.

\- Soft knee to avoid harsh cut lines.



\### 5.2 Downsample chain

Create mips:

\- `BLOOM\_MIP\[0]` = 1/2

\- `BLOOM\_MIP\[1]` = 1/4

\- `BLOOM\_MIP\[2]` = 1/8

\- `BLOOM\_MIP\[3]` = 1/16

(Stop earlier if UI is small.)



Downsample filter:

\- Use a decent filter that preserves highlights (not a naive box).



\### 5.3 Blur each mip

\- Use separable gaussian or Kawase.

\- More blur at smaller mips (or fixed steps per mip).



\### 5.4 Upsample and combine

\- Upsample `BLOOM\_MIP\[n]` into `BLOOM\_MIP\[n-1]` and accumulate with weights.

\- Output final bloom as `BLOOM\_FINAL` (can be `BLOOM\_MIP\[0]` after accumulation).



Deliverable:

\- `BLOOM\_FINAL` looks like a smooth halo, not a smeary UI blur.



---



\## 6) Composite and tone-map to the swapchain

\### 6.1 Composite in HDR

Compute:

\- `UI\_HDR\_OUT = UI\_HDR\_BASE + (BLOOM\_FINAL \* bloomIntensity)`



\### 6.2 Tone map

\- Apply a tone mapper (ACES-ish or a simple filmic curve).

\- Optional: exposure control (even fixed exposure is fine for UI-only).



\### 6.3 Present

\- Output to swapchain backbuffer.



DX11:

\- Set RTV = backbuffer, draw fullscreen triangle/quad with composite shader.



DX12:

\- Transition backbuffer to RTV, draw fullscreen triangle with composite PSO.



Deliverable:

\- Final UI shows natural emissive glow with stable tuning.



---



\## 7) DX11 backend implementation steps

1\. Create device resources:

&nbsp;  - Textures + RTV/SRV for `UI\_HDR\_BASE`, `UI\_HDR\_EMISSIVE`, bloom mips.

2\. Create states:

&nbsp;  - Rasterizer, depth disabled, blend states (premult recommended).

3\. Create shaders:

&nbsp;  - ImGui MRT shader (BASE + EMISSIVE).

&nbsp;  - Prefilter/downsample/blur/upsample shaders.

&nbsp;  - Composite + tone map shader.

4\. Hook frame flow:

&nbsp;  - BeginFrame → render ImGui MRT → bloom chain → composite to backbuffer → Present.

5\. Resize handling:

&nbsp;  - Recreate all RTs when swapchain size changes.



---



\## 8) DX12 backend implementation steps

1\. Build a shared DX12 resource manager:

&nbsp;  - ID3D12Resource for each RT + SRV/RTV descriptors.

&nbsp;  - Descriptor heaps: SRV heap (shader visible), RTV heap.

2\. Create PSOs:

&nbsp;  - ImGui MRT PSO (2 RTVs RGBA16F).

&nbsp;  - Prefilter/downsample/blur/upsample PSOs.

&nbsp;  - Composite PSO to swapchain.

3\. Barriers and states:

&nbsp;  - Track transitions for each texture:

&nbsp;    - RTV ↔ SRV (RENDER\_TARGET ↔ PIXEL\_SHADER\_RESOURCE)

&nbsp;    - backbuffer PRESENT ↔ RENDER\_TARGET

4\. Command list flow per frame:

&nbsp;  - Transition UI RTs to RTV → clear → render ImGui

&nbsp;  - Transition UI emissive to SRV → bloom passes (each pass: RTV target + SRV input)

&nbsp;  - Transition backbuffer to RTV → composite/tone map pass → transition to PRESENT

5\. Descriptor binding:

&nbsp;  - Use root signature that supports:

&nbsp;    - SRV table for inputs (font atlas, UI textures, bloom inputs)

&nbsp;    - constants (threshold, knee, bloom weights, tone mapping params)



---



\## 9) Make visuals match between DX11 and DX12

1\. Use the same:

&nbsp;  - render target formats

&nbsp;  - sampling modes (linear clamp is typical)

&nbsp;  - shader code (compile to both)

&nbsp;  - bloom weights and mip count

2\. Confirm sRGB handling:

&nbsp;  - If using \_SRGB swapchain, ensure final output shader writes linear and HW converts.

&nbsp;  - Or manually gamma encode if using UNORM without sRGB (pick one path and stick to it).



---



\## 10) Debug views (required to tune fast)

Add an ImGui “PostFX Debug” window with toggles to show:

\- `UI\_HDR\_BASE`

\- `UI\_HDR\_EMISSIVE`

\- `BLOOM\_MIP\[0..n]`

\- `BLOOM\_FINAL`

\- Final composited image



Add live sliders:

\- emissive global multiplier

\- threshold

\- knee

\- bloom intensity

\- blur radius / steps

\- tone map exposure



---



\## 11) Performance and quality knobs

\- Bloom resolution:

&nbsp; - Start bloom at 1/2 res for speed, go full-res only if needed.

\- Mip count:

&nbsp; - 3–5 mips usually enough for UI.

\- Blur:

&nbsp; - Kawase is cheap; gaussian is predictable.

\- Avoid reallocations:

&nbsp; - Keep RTs persistent; rebuild only on resize.



---



\## 12) “Done” criteria (what to test)

\- Text glow is clean (no weird halos around glyph edges).

\- Thin lines don’t explode into fat blurry streaks.

\- Glow radius scales with DPI/UI scale.

\- DX11 and DX12 match closely (side-by-side capture comparisons).

\- Resizing doesn’t leak resources or stutter badly.



---



\## Minimal deliverables order (build in this sequence)

1\. UI rendered to `UI\_HDR\_BASE` (RGBA16F) + tone map to backbuffer.

2\. Add `UI\_HDR\_EMISSIVE` and a simple threshold + single blur (quick test).

3\. Replace with full bloom pyramid (downsample/blur/upsample).

4\. Fork ImGui to carry emissive strength per vertex or per drawcmd.

5\. Final tuning + debug views + parity between DX11/DX12.
---

