#include "quicknii.hpp"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // make this window non-resizable

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(720, 720, "QuickNii", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup ImPlot context
    ImPlot::CreateContext();

    // Our state
    ImVec4 clear_color = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float xs1[1001], ys1[1001];
        for (int i = 0; i < 1001; ++i) {
            xs1[i] = i * 0.001f;
            ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
        }

        static double xs2[20], ys2[20];
        for (int i = 0; i < 20; ++i) {
            xs2[i] = i * 1/19.0f;
            ys2[i] = xs2[i] * xs2[i];
        }


        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);
        ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoMove | 
                                        ImGuiWindowFlags_NoCollapse | 
                                        ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoTitleBar | 
                                        ImGuiWindowFlags_NoDecoration); // No title, no border, no resizes

        // Axis flags for the ImPlots
        static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock | 
                                            ImPlotAxisFlags_NoGridLines | 
                                            ImPlotAxisFlags_NoTickMarks | 
                                            ImPlotAxisFlags_NoDecorations;
    
        // Create three ImPlots in a 2x2 grid
        for (int i = 0; i < 4; ++i) {
            // If this is the fourth iteration (i == 3), create an empty space instead of a plot
            if (i == 3) {
                // 3D slider here, now is a dummy
                // ImGui::Dummy(ImVec2(350, 350));
                slider3d("3D Slider");

            } else {
                // Create ImPlots
                if (ImPlot::BeginPlot(("plot" + std::to_string(i)).c_str(), ImVec2(350, 350), ImPlotAxisFlags_NoDecorations)) {
                    
                    // Stying the plot
                    ImPlot::SetupAxes(NULL, NULL, axes_flags, axes_flags);
                    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));

                    // 1 means sagittal, 2 means coronal, 3 means axial
                    // Implementation of plot functions here 

                    ImPlot::EndPlot();
                }
            }
            // If this is not the last plot in the row, create a same-line spacer
            if (i % 2 == 0) {
                ImGui::SameLine();
            }
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
