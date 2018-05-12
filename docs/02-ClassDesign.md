# Class Design

## Classes

### `Game`

- Initialize OpenGL
- Initialize Resources
- Process Inputs
- Render All

### `ResourceManager` (Singleton)

- Create and store resources

### `Camera`

- Manage perspective and view matrices, act as a camera

### `Shader`

- Host a shader program

### `Texture`

- Host a texture

### `SpriteRenderer` (Abstract)

- To render a sprite, its `initRenderData` function must be implemented

- Current Renderers:

    - `CubeRenderer` Render a cube
