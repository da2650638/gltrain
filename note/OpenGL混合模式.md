# OpenGL 混合模式

## **`glBlendFunc` 参数**

`glBlendFunc` 接受两个参数，分别定义源颜色（即要绘制的颜色）和目标颜色（帧缓冲中已有的颜色）的权重。

### **常用选项**

1. **`GL_ZERO`**
   - 值为 0，完全不影响最终颜色。

2. **`GL_ONE`**
   - 值为 1，保留颜色的全部贡献。

3. **`GL_SRC_COLOR`**
   - 使用源颜色的每个通道值作为权重。

4. **`GL_ONE_MINUS_SRC_COLOR`**
   - 使用 `1 - 源颜色` 的每个通道值作为权重。

5. **`GL_DST_COLOR`**
   - 使用目标颜色的每个通道值作为权重。

6. **`GL_ONE_MINUS_DST_COLOR`**
   - 使用 `1 - 目标颜色` 的每个通道值作为权重。

7. **`GL_SRC_ALPHA`**
   - 使用源颜色的 alpha 值作为权重。

8. **`GL_ONE_MINUS_SRC_ALPHA`**
   - 使用 `1 - 源颜色 alpha` 作为权重。

9. **`GL_DST_ALPHA`**
   - 使用目标颜色的 alpha 值作为权重。

10. **`GL_ONE_MINUS_DST_ALPHA`**
    - 使用 `1 - 目标颜色 alpha` 作为权重。

11. **`GL_CONSTANT_COLOR`**
    - 使用预定义的常量颜色作为权重（通过 `glBlendColor` 设置）。

12. **`GL_ONE_MINUS_CONSTANT_COLOR`**
    - 使用 `1 - 常量颜色` 作为权重。

13. **`GL_CONSTANT_ALPHA`**
    - 使用预定义的常量 alpha 值作为权重。

14. **`GL_ONE_MINUS_CONSTANT_ALPHA`**
    - 使用 `1 - 常量 alpha` 作为权重。

---

## **`glBlendEquation` 参数**

`glBlendEquation` 定义源和目标颜色之间的数学运算，默认值是 `GL_FUNC_ADD`（相加）。

### **常用选项**

1. **`GL_FUNC_ADD`**
   - 默认值：`结果 = 源 * 源权重 + 目标 * 目标权重`

2. **`GL_FUNC_SUBTRACT`**
   - 结果：`结果 = 源 * 源权重 - 目标 * 目标权重`

3. **`GL_FUNC_REVERSE_SUBTRACT`**
   - 结果：`结果 = 目标 * 目标权重 - 源 * 源权重`

4. **`GL_MIN`**
   - 结果：`结果 = min(源, 目标)`（按通道取最小值）

5. **`GL_MAX`**
   - 结果：`结果 = max(源, 目标)`（按通道取最大值）

---

## **常见混合模式**

1. **普通透明混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);
   ```
   - 应用场景：处理透明纹理，按 alpha 值进行混合。

2. **加法混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glBlendEquation(GL_FUNC_ADD);
   ```
   - 应用场景：用于光照效果、粒子系统等叠加效果。

3. **减法混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glBlendEquation(GL_FUNC_SUBTRACT);
   ```
   - 应用场景：用于反向叠加效果。

4. **乘法混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFunc(GL_DST_COLOR, GL_ZERO);
   glBlendEquation(GL_FUNC_ADD);
   ```
   - 应用场景：实现暗化、阴影效果。

5. **屏幕混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
   glBlendEquation(GL_FUNC_ADD);
   ```
   - 应用场景：实现光效、柔和叠加效果。

6. **双加权混合**
   ```cpp
   glEnable(GL_BLEND);
   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
   glBlendEquation(GL_FUNC_ADD);
   ```
   - 应用场景：分别为颜色和 alpha 通道设置权重。

---

## **其他相关函数**

1. **`glBlendColor`**
   - 用于设置 `GL_CONSTANT_COLOR` 和 `GL_CONSTANT_ALPHA` 的值：
     ```cpp
     glBlendColor(0.5f, 0.5f, 0.5f, 0.5f); // 设置常量颜色为半透明灰色
     ```

2. **`glBlendFuncSeparate`**
   - 允许分别设置颜色和 alpha 通道的混合函数：
     ```cpp
     glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
     ```

---

## **注意事项**

- 混合是按像素进行的，可能会导致性能开销。
- 混合需要正确的绘制顺序，通常需要先绘制不透明对象，再绘制半透明对象。
- 混合结果会受到帧缓冲中已有的颜色影响。

通过选择合适的 `glBlendFunc` 和 `glBlendEquation` 参数，可以实现各种复杂的透明效果和图像叠加效果。

## 源颜色说明
在 OpenGL 的上下文中，**要绘制的颜色**（源颜色）通常指的是在执行绘制命令（如 `glDrawArrays` 或 `glDrawElements`）时，片段着色器（Fragment Shader）输出的颜色值。这个颜色值会参与到 OpenGL 的混合操作，并最终决定写入帧缓冲区的颜色。

### 具体来说：
1. **片段着色器输出颜色**
   - 在 OpenGL 管线中，片段着色器的主要任务是为屏幕上的每个片段（像素）计算颜色值。
   - 例如，在片段着色器中：
     ```glsl
     out vec4 outColor;
     outColor = texture(texture0, fragTexCoord) * fragColor;
     ```
     - `outColor` 是片段着色器的输出。
     - 它代表了当前片段在无混合的情况下应该写入帧缓冲区的颜色。

2. **参与混合的颜色**
   - 如果启用了混合（`glEnable(GL_BLEND)`），片段着色器的输出颜色会被当作源颜色（Source Color），与目标颜色（Destination Color）按照指定的混合函数和混合方程进行计算：
     ```cpp
     ResultColor = SourceFactor * SourceColor + DestinationFactor * DestinationColor;
     ```
   - 其中，`SourceColor` 就是片段着色器的输出颜色。

3. **纹理和片段颜色的结合**
   - 在实际应用中，片段着色器的输出颜色往往由以下两部分计算得来：
     - **纹理颜色**：从绑定的纹理中采样得到。
     - **顶点颜色或统一颜色**：可以作为对纹理颜色的调制或叠加。
   - 例如：
     ```glsl
     vec4 texelColor = texture(texture0, fragTexCoord); // 从纹理中采样颜色
     outColor = texelColor * fragColor;                // 与顶点颜色或传入的颜色相乘
     ```

4. **最终显示的颜色**
   - 在无混合的情况下，片段着色器输出的颜色直接覆盖帧缓冲区中的目标颜色。
   - 在启用了混合的情况下，片段着色器输出的颜色会与帧缓冲区的目标颜色按照混合公式结合，生成最终的显示颜色。

---

### 示例流程：
假设你要绘制一个带有透明度的纹理：
1. **片段着色器输出颜色**：
   - 假设输出的颜色是 `vec4(1.0, 0.0, 0.0, 0.5)`，即半透明红色。
2. **目标颜色（帧缓冲区中的颜色）**：
   - 比如目标颜色是 `vec4(0.0, 1.0, 0.0, 1.0)`，即完全不透明的绿色。
3. **混合公式**：
   - 如果混合函数设置为：
     ```cpp
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     ```
   - 则最终颜色计算为：
     ```
     ResultColor = (SourceAlpha * SourceColor) + ((1 - SourceAlpha) * DestinationColor)
                 = (0.5 * vec4(1.0, 0.0, 0.0, 0.5)) + (0.5 * vec4(0.0, 1.0, 0.0, 1.0))
                 = vec4(0.5, 0.5, 0.0, 1.0)
     ```
4. **最终显示的颜色**：
   - 绘制结果为半透明的黄绿色。

---

### 总结：
- **要绘制的颜色**指的是片段着色器输出的颜色值。
- 它通常由纹理颜色和片段颜色的结合产生。
- 如果启用了混合，这个颜色值会作为**源颜色**，与目标颜色结合生成最终的显示颜色。