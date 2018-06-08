## Basic
### 光源

光源可选择点光源或平行光源，在`Light`类中进行配置。

```c
struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
```

将其独立出来，方便添加其他光源，以及太阳光源元素的修改，之后可能随着全局时间变换太阳光方向等模拟真实太阳升起落下。

### 物体材质

不同物体受光材质不同，其漫反射，环境反射和镜面反射度均可能不相同，同一物体不同部分的受光材质也可能不同，一个物体具有漫反射贴图、镜面贴图和反光度贴图三种贴图，我们通过固定材质单元槽位的方式绑定相应的贴图。

```c
uniform sampler2D texture0;  // diffuse
uniform sampler2D texture1;  // specular
uniform sampler2D texture2;  // shininess
```

而不同物体可以给赋予不同的贴图从而达到不同的镜面反射效果。

### Phong光照模型

当前使用Phong光照模型和冯氏着色Phong Shading。

### 模拟白天黑夜
- 太阳按照椭圆轨迹变化：太阳模型和相应的变化
- 光照方向变化
- 天空盒使用两个进行切换，通过apha进行控制
草图...
![light_change.jpg](./assets/light_change.jpg)
