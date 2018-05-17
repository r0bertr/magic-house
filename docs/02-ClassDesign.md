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

### `Model`

- Host a imported model

### `Renderer` (Abstract)

- To render an object, its `initRenderData` function must be implemented

- Current Renderers:

    - `MeshRenderer` Render a mesh of a model
    - `LandRenderer` Render the land
