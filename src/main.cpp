#include <wx/wx.h>
#include <wx/cmdproc.h>
#include <wx/listctrl.h>

#include "commands.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void SetupMenuBar();

    void OnAddButtonClicked(wxCommandEvent &event);
    void OnRemoveButtonClicked(wxCommandEvent &event);

    wxCommandProcessor commandProcessor;
    wxListView *listView;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    listView->InsertColumn(0, "Name");
    listView->SetColumnWidth(0, FromDIP(180));

    SetBackgroundColour(listView->GetBackgroundColour());

    sizer->Add(listView, 1, wxEXPAND);

    auto buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
    auto addButton = new wxButton(this, wxID_ANY, "Add");
    auto removeButton = new wxButton(this, wxID_ANY, "Remove");
    buttonsSizer->AddStretchSpacer();
    buttonsSizer->Add(addButton, 0, wxRIGHT, FromDIP(5));
    buttonsSizer->Add(removeButton);
    buttonsSizer->AddStretchSpacer();

    sizer->Add(buttonsSizer, 0, wxEXPAND | wxALL, FromDIP(5));

    SetSizerAndFit(sizer);

    addButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddButtonClicked, this);
    removeButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveButtonClicked, this);

    SetupMenuBar();
}

void MyFrame::OnAddButtonClicked(wxCommandEvent &event)
{
    wxTextEntryDialog dialog(this, "Enter name", "Add name");
    if (dialog.ShowModal() == wxID_OK)
    {
        auto name = dialog.GetValue();
        auto index = listView->GetItemCount();

        commandProcessor.Submit(new AddListItemCommand(listView, index, name));
    }
}

void MyFrame::OnRemoveButtonClicked(wxCommandEvent &event)
{
    auto index = listView->GetFirstSelected();
    if (index != -1)
    {
        commandProcessor.Submit(new RemoveListItemCommand(listView, index));
    }
}

void MyFrame::SetupMenuBar()
{
    wxMenu *editMenu = new wxMenu();
    editMenu->Append(wxID_UNDO);
    editMenu->Append(wxID_REDO);

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(editMenu, "Edit");

    SetMenuBar(menuBar);

    commandProcessor.SetEditMenu(editMenu);
    commandProcessor.Initialize();

    this->Bind(wxEVT_MENU, [&](wxCommandEvent &event)
               {
                   if (event.GetId() == wxID_UNDO)
                   {
                       commandProcessor.Undo();
                   }
                   else if (event.GetId() == wxID_REDO)
                   {
                       commandProcessor.Redo();
                   } });
}
