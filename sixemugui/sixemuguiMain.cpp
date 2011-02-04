/***************************************************************
 * Name:      sixemuguiMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matlo (mat.lau@laposte.net)
 * Created:   2011-01-12
 * Copyright: Matlo (http://diy-machine.blogspot.com/)
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "sixemuguiMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(sixemuguiFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <glib.h>

#include <pthread.h>
#include <unistd.h>
#include <errno.h>

//helper functions
enum wxbuildinfoformat
{
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(sixemuguiFrame)
const long sixemuguiFrame::ID_STATICTEXT1 = wxNewId();
const long sixemuguiFrame::ID_CHOICE1 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT2 = wxNewId();
const long sixemuguiFrame::ID_CHOICE2 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT3 = wxNewId();
const long sixemuguiFrame::ID_CHOICE3 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT4 = wxNewId();
const long sixemuguiFrame::ID_CHOICE5 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT6 = wxNewId();
const long sixemuguiFrame::ID_CHOICE6 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT7 = wxNewId();
const long sixemuguiFrame::ID_CHOICE7 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT8 = wxNewId();
const long sixemuguiFrame::ID_BUTTON2 = wxNewId();
const long sixemuguiFrame::ID_STATICTEXT10 = wxNewId();
const long sixemuguiFrame::ID_BUTTON1 = wxNewId();
const long sixemuguiFrame::ID_BUTTON3 = wxNewId();
const long sixemuguiFrame::ID_CHECKBOX1 = wxNewId();
const long sixemuguiFrame::ID_PANEL1 = wxNewId();
const long sixemuguiFrame::ID_MENUITEM1 = wxNewId();
const long sixemuguiFrame::idMenuQuit = wxNewId();
const long sixemuguiFrame::idMenuAbout = wxNewId();
const long sixemuguiFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(sixemuguiFrame,wxFrame)
    //(*EventTable(sixemuguiFrame)
    //*)
END_EVENT_TABLE()

static int readCommandResults(const char * argv[], int nb_params, wxString params[], int nb_repeat, wxString results[])
{
    int exit_status = 0;
    char* out = NULL;
    char* err = NULL;
    GError *error = NULL;
    gboolean test;
    char* line;
    char* end;
    int ret = 0;
    char c_param[256];

    test = g_spawn_sync(NULL, (gchar**)argv, NULL, (GSpawnFlags)G_SPAWN_SEARCH_PATH, NULL, NULL, &out, &err, &exit_status, &error);

    if(test)
    {
        line = out;

        printf("%s\n", out);

        for(int i=0; i<nb_params*nb_repeat; ++i)
        {
            strncpy(c_param, params[i%nb_params].mb_str(), sizeof(c_param));

            if((line = strstr(line, c_param)))
            {
                if((end = strstr(line, "\n")))
                {
                    *end = '\0';
                }
                results[i] = wxString(line+strlen(c_param), wxConvUTF8);
                line=end+1;
            }
            else
            {
                if(nb_repeat == 1)
                {
                    ret = -1;
                }
                break;
            }
        }
    }
    else
    {
        ret = -1;
    }

    if(out) g_free(out);
    if(err) g_free(err);
    if(error) g_free(error);

    return ret;
}

static char hci[7];
static char bad[18];

static const char *sixaxis_device[] = { "gksudo", "sixaddr", NULL };

static const char *hciconfig_all[] = { "hciconfig", "-a", hci, NULL };
static const char *hciconfig_revision[] = { "gksudo", "hciconfig", hci, "revision", NULL };

static const char *bdaddr[] = { "bdaddr", "-i", hci, NULL };
static const char *bdaddr_modify[] = { "sudo", "bdaddr", "-r", "-i", hci, bad, NULL };

void sixemuguiFrame::readSixaxis()
{
    wxString params[2] = {_("Current Bluetooth master: "), _("Current Bluetooth Device Address: ")};
    wxString results[14];

    int res = readCommandResults(sixaxis_device, 2, params, 7, results);

    if(res != -1)
    {
        for(int i=0; i<13; i+=2)
        {
            if(results[i].IsEmpty())
            {
                break;
            }
            Choice2->Append(results[i].MakeUpper());
            Choice1->Append(results[i+1].MakeUpper());
        }
    }
}

void sixemuguiFrame::readDongles()
{
    wxString params1[3] = {_("BD Address: "), _("Name: "), _("Manufacturer: ")};
    wxString results1[3];
    wxString params2[1] = {_("Chip version: ")};
    wxString results2[1];
    int res;

    for(int i=0; i<256; ++i)
    {
        snprintf(hci, sizeof(hci), "hci%d", i);
        res = readCommandResults(hciconfig_all, 3, params1, 1, results1);

        if(res != -1)
        {
            Choice3->Append(results1[0].Left(17));

            Choice5->Append(results1[1]);

            Choice6->Append(results1[2]);

            res = readCommandResults(hciconfig_revision, 1, params2, 1, results2);

            if(res != -1)
            {
                Choice7->Append(results2[0]);
            }
            else
            {
                Choice7->Append(_(""));
            }
        }
        else
        {
            break;
        }
    }
}

int sixemuguiFrame::setDongleAddress()
{
    int i;
    int j = 0;
    unsigned int k;
    wxString params1[1] = {_("Address changed - ")};
    wxString results1[1];
    wxString params2[1] = {_("Device address: ")};
    wxString results2[1];
    int res;

    if(Choice1->GetStringSelection().IsEmpty())
    {
        wxMessageBox( wxT("Please select a Sixaxis Address!"), wxT("Error"), wxICON_ERROR);
        return -1;
    }
    else if(Choice3->GetStringSelection().IsEmpty())
    {
        wxMessageBox( wxT("Please select a Bluetooth Dongle!"), wxT("Error"), wxICON_ERROR);
        return -1;
    }

    for(k=0; k<Choice3->GetCount(); ++k)
    {
        if(Choice3->GetString(k) == Choice1->GetStringSelection())
        {
            wxMessageBox( wxT("Address already used!"), wxT("Error"), wxICON_ERROR);
            return -1;
        }
    }

    i = Choice3->GetSelection();
    snprintf(hci, sizeof(hci), "hci%d", i);
    strncpy( bad, Choice1->GetStringSelection().mb_str(), 18 );

    res = readCommandResults(bdaddr_modify, 1, params1, 1, results1);

    if(res != -1)
    {
        wxMessageBox( results1[0], wxT("Success"), wxICON_INFORMATION);
    }

    while(readCommandResults(bdaddr, 1, params2, 1, results2) == -1 && j<50)
    {
        usleep(100000);
        j++;
    }

    if(results2[0] !=  Choice1->GetStringSelection())
    {
        wxMessageBox( wxT("Read address after set: ko!"), wxT("Error"), wxICON_ERROR);
    }
    else
    {
        wxMessageBox( wxT("Read address after set: seems ok!"), wxT("Success"), wxICON_INFORMATION);
    }

    return 0;
}

void sixemuguiFrame::refresh()
{
    Choice1->Clear();
    Choice2->Clear();
    Choice3->Clear();
    Choice5->Clear();
    Choice6->Clear();
    Choice7->Clear();
    readSixaxis();
    readDongles();
    if(Choice1->GetCount() == 0)
    {
        wxMessageBox( wxT("No Sixaxis Detected!\nSixaxis usb wire plugged?"), wxT("Error"), wxICON_ERROR);
    }
    else
    {
        if(Choice1->GetSelection() < 0)
        {
            Choice1->SetSelection(0);
        }
        if(Choice2->GetSelection() < 0)
        {
            Choice2->SetSelection(Choice1->GetSelection());
        }
    }
    if(Choice3->GetCount() == 0)
    {
        wxMessageBox( wxT("No Bluetooth Dongle Detected!"), wxT("Error"), wxICON_ERROR);
    }
    else
    {
        if(Choice3->GetSelection() < 0)
        {
            Choice3->SetSelection(0);
            Choice5->SetSelection(0);
            Choice6->SetSelection(0);
            Choice7->SetSelection(0);
        }
    }
}

sixemuguiFrame::sixemuguiFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(sixemuguiFrame)
    wxStaticBoxSizer* StaticBoxSizer2;
    wxFlexGridSizer* FlexGridSizer4;
    wxMenuItem* MenuItem2;
    wxStaticBoxSizer* StaticBoxSizer4;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer2;
    wxMenu* Menu1;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxMenuBar* MenuBar1;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("Sixemugui"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetMinSize(wxSize(400,575));
    SetMaxSize(wxSize(-1,-1));
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer1 = new wxFlexGridSizer(4, 1, 0, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Sixaxis"));
    FlexGridSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Address"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice1 = new wxChoice(Panel1, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    FlexGridSizer2->Add(Choice1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("PS3 address"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice2 = new wxChoice(Panel1, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    FlexGridSizer2->Add(Choice2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Dongle"));
    FlexGridSizer3 = new wxFlexGridSizer(5, 2, 0, 0);
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Address"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice3 = new wxChoice(Panel1, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    FlexGridSizer3->Add(Choice3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice5 = new wxChoice(Panel1, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
    FlexGridSizer3->Add(Choice5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Manufacturer"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer3->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice6 = new wxChoice(Panel1, ID_CHOICE6, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
    FlexGridSizer3->Add(Choice6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, _("Chip version"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer3->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice7 = new wxChoice(Panel1, ID_CHOICE7, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE7"));
    FlexGridSizer3->Add(Choice7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer3->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Set Dongle Address"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    FlexGridSizer3->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("emu"));
    FlexGridSizer4 = new wxFlexGridSizer(1, 3, 0, 0);
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    FlexGridSizer4->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer4->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3->Add(FlexGridSizer4, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("emuclient"));
    FlexGridSizer5 = new wxFlexGridSizer(1, 3, 0, 0);
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Button3->Disable();
    FlexGridSizer5->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CheckBox1 = new wxCheckBox(Panel1, ID_CHECKBOX1, _("Grab mouse"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(true);
    FlexGridSizer5->Add(CheckBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(FlexGridSizer1);
    FlexGridSizer1->Fit(Panel1);
    FlexGridSizer1->SetSizeHints(Panel1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Refresh\tF5"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[2] = { -1, 20 };
    int __wxStatusBarStyles_1[2] = { wxSB_NORMAL, wxSB_NORMAL };
    StatusBar1->SetFieldsCount(2,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(2,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnSelectSixaxisBdaddr);
    Connect(ID_CHOICE2,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnSelectPS3Bdaddr);
    Connect(ID_CHOICE3,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnSelectBtDongle);
    Connect(ID_CHOICE5,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnChoice5Select);
    Connect(ID_CHOICE6,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnChoice6Select);
    Connect(ID_CHOICE7,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnChoice7Select);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sixemuguiFrame::OnButton2Click);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sixemuguiFrame::OnButton1Click);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sixemuguiFrame::OnButton3Click);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnSelectRefresh);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sixemuguiFrame::OnAbout);
    //*)

    launched = false;

    refresh();
}

sixemuguiFrame::~sixemuguiFrame()
{
    //(*Destroy(sixemuguiFrame)
    //*)
}

void sixemuguiFrame::OnQuit(wxCommandEvent& event)
{
    int answer;
    if(launched)
    {
        answer = wxMessageBox(_("Emu process will be killed!"), _("Confirm"), wxNO | wxCANCEL);
        if (answer == wxYES)
        {
            g_spawn_command_line_sync ("killall emu", NULL, NULL, NULL, NULL);
        }
        else
        {
            return;
        }
    }
    Close();
}

void sixemuguiFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void sixemuguiFrame::OnSelectSixaxisBdaddr(wxCommandEvent& event)
{
    Choice2->SetSelection(Choice1->GetSelection());
}

void sixemuguiFrame::OnSelectPS3Bdaddr(wxCommandEvent& event)
{
    Choice1->SetSelection(Choice2->GetSelection());
}

void sixemuguiFrame::OnSelectBtDongle(wxCommandEvent& event)
{
    Choice5->SetSelection(Choice3->GetSelection());
    Choice6->SetSelection(Choice3->GetSelection());
    Choice7->SetSelection(Choice3->GetSelection());
}

void sixemuguiFrame::OnSelectRefresh(wxCommandEvent& event)
{
    refresh();
}

void sixemuguiFrame::OnButton2Click(wxCommandEvent& event)
{
    int answer = wxMessageBox(_("Did you saved your dongle address?"), _("Confirm"), wxYES_NO | wxCANCEL);
    if (answer == wxYES)
    {
        if(setDongleAddress() != -1)
        {
            refresh();
        }
    }
    else if (answer == wxNO)
    {
        wxMessageBox(_("Please save it!"), _("Info"));
    }
}

void sixemuguiFrame::OnChoice5Select(wxCommandEvent& event)
{
    Choice3->SetSelection(Choice5->GetSelection());
    Choice6->SetSelection(Choice5->GetSelection());
    Choice7->SetSelection(Choice5->GetSelection());
}

void sixemuguiFrame::OnChoice6Select(wxCommandEvent& event)
{
    Choice3->SetSelection(Choice6->GetSelection());
    Choice5->SetSelection(Choice6->GetSelection());
    Choice7->SetSelection(Choice6->GetSelection());
}

void sixemuguiFrame::OnChoice7Select(wxCommandEvent& event)
{
    Choice3->SetSelection(Choice7->GetSelection());
    Choice5->SetSelection(Choice7->GetSelection());
    Choice6->SetSelection(Choice7->GetSelection());
}

static char emu_bdaddr[18];
static char bt_device[4];

static const char *emu_command[] = { "emu", emu_bdaddr, bt_device, "0", NULL };

typedef enum
{
    E_CONNECTING,
    E_CONNECTED,
    E_DISCONNECTED,
    E_ERROR
} e_emu_state;

e_emu_state emu_state = E_DISCONNECTED;

void* emu_thread(void* arg)
{
    GError *error = NULL;
    gboolean test;
    GPid child_pid;
    int err = -1;
    char tmp[1024];
    int ret;

    test = g_spawn_async_with_pipes(NULL, (gchar**)emu_command, NULL, (GSpawnFlags)(G_SPAWN_SEARCH_PATH), NULL, NULL, &child_pid, NULL, NULL, &err,  &error);

    if(error)
    {
        printf("%s\n", error->message);
    }

    if(test)
    {
        while(1)
        {
            ret = read(err, tmp, 1024);
            tmp[1023] = '\0';
            if(ret == -1 && errno != EAGAIN)
            {
                emu_state = E_ERROR;
                break;
            }
            if(ret > 0)
            {
                if(strstr(tmp, "connected"))
                {
                    emu_state = E_CONNECTED;
                    break;
                }
                else if(strstr(tmp, "can't connect to"))
                {
                    emu_state = E_ERROR;
                    break;
                }
            }
            sleep(1);
        }
    }

    if(error) g_free(error);
    if(err > -1) close(err);
    pthread_exit (0);
}

void sixemuguiFrame::OnButton1Click(wxCommandEvent& event)
{
    pthread_t thread;
    pthread_attr_t thread_attr;
    char command[64];

    if(!launched)
    {
        /*
         * This allows not to launch the emu process as root.
         */
        snprintf(command, 64, "gksudo hciconfig hci%d class 0x508", Choice3->GetSelection());
        g_spawn_command_line_sync (command, NULL, NULL, NULL, NULL);
        /*
         * Update variables to be read by the thread.
         */
        strncpy(emu_bdaddr, Choice2->GetStringSelection().mb_str(), 18 );
        snprintf(bt_device, 4, "%d", Choice3->GetSelection());
        /*
         * Launches the emu process.
         */
        emu_state = E_CONNECTING;
        pthread_attr_init(&thread_attr);
        pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
        pthread_create( &thread, &thread_attr, emu_thread, NULL);

        while(emu_state == E_CONNECTING)
        {
            sleep(1);
        }
        if(emu_state == E_CONNECTED)
        {
            /*
             * Update the GUI.
             */
            Button1->SetLabel(_("Stop"));
            launched = true;
            Button3->Enable();
            Button2->Disable();
            wxMessageBox(_("Connected!\nStart emuclient now."), _("Info"));
        }
        if(emu_state == E_ERROR)
        {
            wxMessageBox( wxT("Connection error!\nDid you set the dongle address?\nIf yes, try another dongle!"), wxT("Error"), wxICON_ERROR);
        }
    }
    else
    {
        /*
         * Kills the emu process.
         */
        g_spawn_command_line_sync ("killall emu", NULL, NULL, NULL, NULL);
        Button1->SetLabel(_("Start"));
        launched = false;
        Button3->Disable();
        Button2->Enable();
    }
}

void sixemuguiFrame::OnButton3Click(wxCommandEvent& event)
{
    /*
     * Launches the emuclient in a terminal.
     */
    if(CheckBox1->IsChecked())
    {
        Button3->Disable();
        g_spawn_command_line_sync ("gnome-terminal -e emuclient", NULL, NULL, NULL, NULL);
        Button3->Enable();
    }
    else
    {
        Button3->Disable();
        g_spawn_command_line_sync ("gnome-terminal -e \"emuclient nograb\"", NULL, NULL, NULL, NULL);
        Button3->Enable();
    }
}
