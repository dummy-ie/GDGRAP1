#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        double x = (double)0.5;
        double y = (double)0;
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < 5; i++)
        {
            double newX = x * cos((72 * i) * (double)(M_PI / 180)) - y * sin((72 * i) * (double)(M_PI / 180)); 
            double newY = y * cos((72 * i) * (double)(M_PI / 180)) + x * sin((72 * i) * (double)(M_PI / 180)); 
            glVertex2f(newX, newY);
        }
        
        // glVertex2f(-0.4f, 0.5f);
        // glVertex2f(-0.4f, 0.f);
        // glVertex2f(0.f, -0.2f);
        // glVertex2f(0.3f, 0.25f);
        // glVertex2f(0.f, 0.7f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
