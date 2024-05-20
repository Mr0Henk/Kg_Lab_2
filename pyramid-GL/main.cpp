// подключение всех библиотек

//  ********    !!!ВООБЩЕ НЕ ТРОГАТЬ!!!   *********
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//  ********    !!!ВООБЩЕ НЕ ТРОГАТЬ!!!   *********

using namespace glm;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) // функция выхода от кнопки ESC
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
    // и приложение после этого закроется
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void renderPyramid(float rotation, float scale, float posX, float posY, float posZ,float red, float green,float blue,bool Trans) {


    //  ********    это база! не трогать!   *********
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер глубины

    glMatrixMode(GL_PROJECTION); // режим матрицы - проекция (+- линза)
    glLoadIdentity(); // сброс матрицы до "по умолчанию"

    // создаём матрицу модели
    glm::mat4 projection;
    projection = glm::perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // колдунсва с перспективой
    glMultMatrixf(value_ptr(projection)); // умножение матриц

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //  ********    это база! не трогать!   *********


    mat4 view = lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(posX, posY, posZ));                   // Изменение координат
    model = rotate(model, radians(rotation), vec3(0.0f, 1.0f, 0.0f));   // Изменение поворота
    model = glm::scale(model, vec3(scale, scale, scale));               // Изменение масштаба
    mat4 mvp = view * model;    //умнажаем матрицы

    //*** !не трогать! ***
    glLoadMatrixf(value_ptr(mvp)); // колдунсва
    //*** !не трогать! ***

    GLfloat pyramidVertices[] = { //координаты всех точек вершин
        // задняя стена
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // левая стена
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        // передняя стена
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // правая стена
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };
    int Trans_index = 0;
    // рисуем красивую (кастыльную) белую рамку

    if (Trans == false)
    {
        Trans_index = 0;
        glColor3f(red, green, blue);
    }
    else
    {
        Trans_index = 1;
        glColor3f(1.0f, 1.0f, 1.0f);
    }


    glBegin(GL_LINE_LOOP);

    glVertex3f(0.0f, 1.0f, 0.0f); //потолок
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);


    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();


    glColor3f(red, green, blue); //цвет назначаем Красный|Зелёный|Сний
    //*******   это матричный художник! не трогать!   *******
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, Trans_index, pyramidVertices);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableClientState(GL_VERTEX_ARRAY);
    //*******   это матричный художник! не трогать!   *******
}

int main() {
    //  ********    это база! не трогать!   *********
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Pyramid", NULL, NULL);
    if (window == NULL) {
        std::cout << "Epic GLFW Failure" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //  ********    это база! не трогать!   *********

    glEnable(GL_DEPTH_TEST); //тест глубины OpenGL

    // объявляем значения переменых по умолчанию
    float initialRotation = 0.0f; // знач Поворот
    float initialScale = 1.0f; // знач Масштаб
    float initialPosX = 0.0f; // знач X
    float initialPosY = 0.0f; // знач Y
    float initialPosZ = 0.0f; // знач Z

    //цвет по умолчанию
    float initialRed = 0.3f;
    float initialGreen = 0.3f;
    float initialBlue = 0.3f;

    bool Trans = false;


    //то что назначает пользователь
    float rotation = initialRotation; // поворот
    float scale = initialScale; // масштаб

    // цвета
    float red = initialRed;
    float green = initialGreen;
    float blue = initialBlue;


    float posX = initialPosX; // коо-
    float posY = initialPosY; // рди-
    float posZ = initialPosZ; // наты

    while (!glfwWindowShouldClose(window)) { // цыкл "пока не окно не закрыли"
        glfwSetKeyCallback(window, key_callback); // если нажали ESC - прога завершаеться

        glfwPollEvents(); // бесконечный рисовальщик

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Panel whith Sliders");//название панельки

        // добавление все нужные ползунки
        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("Scale", &scale, 0.1f, 3.0f);
        ImGui::SliderFloat("The X-axis", &posX, -10.0f, 10.0f);
        ImGui::SliderFloat("The Y-axis", &posY, -10.0f, 10.0f);
        ImGui::SliderFloat("The Z-axis", &posZ, -10.0f, 10.0f);
        // ползунки цвета
        ImGui::SliderFloat("RED", &red, 0.0f, 1.0f);
        ImGui::SliderFloat("GREEN", &green, 0.0f, 1.0f);
        ImGui::SliderFloat("BLUE", &blue, 0.0f, 1.0f);

        if (ImGui::Button("Reset")) { // кнопка сброса до "по умолчанию"
            rotation = initialRotation;
            scale = initialScale;
            posX = initialPosX;
            posY = initialPosY;
            posZ = initialPosZ;
            red = initialRed;
            green = initialGreen;
            blue = initialBlue;
        }
        if (ImGui::Button("Transparency")) { // кнопка вкл/выкл прозрачность
            switch(Trans) {
            case true:
                Trans = false;
                break;
            case false:
                Trans = true;
                break;
            }
        }
        ImGui::End();

        renderPyramid(rotation, scale, posX, posY, posZ,red,green,blue,Trans); // рендер пирамидки



        //  ********    это база! не трогать!   *********
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window); // буфферация
        //  ********    это база! не трогать!   *********
    }


    //  ********    это база! не трогать!   *********
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    //  ********    это база! не трогать!   *********

    return 0;
}

