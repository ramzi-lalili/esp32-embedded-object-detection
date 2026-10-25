# Edge Impulse Model

The model is not stored in this repository because its generated files are large.

It is integrated into the project as an **Arduino library** exported from Edge Impulse.

## Information to Complete

| Item | Value |
|---|---|
| Edge Impulse Project | (link) |
| Type | Image Classification / Object Detection |
| Classes | (list) |
| Dataset Size | (number of images) |
| Input Resolution | (e.g., 96×96) |
| Accuracy | (e.g., 92%) |

## Exporting the Library

1. In Edge Impulse, navigate to **Deployment > Arduino library > Build**.
2. Download the generated `.zip` file.
3. In Arduino IDE, select `Sketch > Include Library > Add .ZIP Library`.

## Library Name

The sketch includes `<SECURTI_inferencing.h>`. The header filename depends on the name of the Edge Impulse project and follows this format:

`<project_name>_inferencing.h`

If your library has a different name (for example, `IMAGE_inferencing.h`), update the `#include` directive in the `.ino` file to match the actual header filename.
