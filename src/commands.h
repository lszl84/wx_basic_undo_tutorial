#pragma once

#include <wx/cmdproc.h>

class AddListItemCommand : public wxCommand
{
public:
    AddListItemCommand(wxListView *listView, int index, const wxString &name)
        : listView(listView), index(index), name(name), wxCommand(true, "Add Item")
    {
    }

    virtual bool Do() override
    {
        listView->InsertItem(index, name);
        return true;
    }

    virtual bool Undo() override
    {
        listView->DeleteItem(index);
        return true;
    }

private:
    wxListView *listView;
    int index;
    wxString name;
};

class RemoveListItemCommand : public wxCommand
{
public:
    RemoveListItemCommand(wxListView *listView, int index)
        : listView(listView), index(index), wxCommand(true, "Remove Item")
    {
        name = listView->GetItemText(index);
    }

    virtual bool Do() override
    {
        listView->DeleteItem(index);
        return true;
    }

    virtual bool Undo() override
    {
        listView->InsertItem(index, name);
        return true;
    }

private:
    wxListView *listView;
    int index;
    wxString name;
};