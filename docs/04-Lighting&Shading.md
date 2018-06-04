## Basic
### 光源

目前仅一个太阳光源（平行光）

```
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;
```

将其独立出来，方便添加其他光源，以及太阳光源元素的修改，之后可能随着全局时间变换太阳光方向等模拟真实太阳升起落下。

### 物体材质

不同物体受光材质不同，其漫反射，环境反射和镜面反射度均可能不相同，同一物体不同部分的受光材质也可能不同，因此采用了纹理贴图作为物体材质的漫反射和环境反射贴图，而由于我们的场景物体几乎不存在镜面反射，因此不使用镜面反射贴图，直接让同一物体的镜面反射度和反光度取值相同：

```
struct Material {
    vec3 specular;    
    float shininess;
};

uniform Material material;
```

而不同物体可以给material的镜面反射度和反光度赋不同值从而达到不同的镜面反射效果。

### Phong光照模型

当前使用Phong光照模型和冯氏着色Phong Shading。



