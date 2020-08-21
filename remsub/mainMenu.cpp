#include <mainMenu.hpp>
#include <Vars/Vars.h>
#include <imguiDormon/imgui.h>
#include <Project.hpp>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

void mainMenu(vars::Vars&vars){
  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("file")){
      if(ImGui::MenuItem("open")){
        igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
      }
      if(ImGui::MenuItem("save")){
        vars.get<Project>("project")->save(vars.getString("projectName"));
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // display
  if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) 
  {
    // action if OK
    if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
    {
      std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
      //TODO
    }
    // close
    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }
}
