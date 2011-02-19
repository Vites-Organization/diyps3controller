/***************************************************************
 * Name:      sixstatusMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matlo (mat.lau@laposte.net)
 * Created:   2011-02-17
 * Copyright: Matlo (http://diy-machine.blogspot.com/)
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "sixstatusMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(sixstatusFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;
#include <sstream>
#include <wx/timer.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

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

//(*IdInit(sixstatusFrame)
const long sixstatusFrame::ID_STATICTEXT1 = wxNewId();
const long sixstatusFrame::ID_GAUGE1 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT4 = wxNewId();
const long sixstatusFrame::ID_GAUGE2 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT3 = wxNewId();
const long sixstatusFrame::ID_GAUGE3 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT2 = wxNewId();
const long sixstatusFrame::ID_GAUGE4 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT5 = wxNewId();
const long sixstatusFrame::ID_GAUGE5 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT6 = wxNewId();
const long sixstatusFrame::ID_GAUGE6 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT7 = wxNewId();
const long sixstatusFrame::ID_GAUGE7 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT8 = wxNewId();
const long sixstatusFrame::ID_GAUGE8 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT9 = wxNewId();
const long sixstatusFrame::ID_GAUGE9 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT10 = wxNewId();
const long sixstatusFrame::ID_GAUGE10 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT11 = wxNewId();
const long sixstatusFrame::ID_GAUGE11 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT12 = wxNewId();
const long sixstatusFrame::ID_GAUGE12 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT13 = wxNewId();
const long sixstatusFrame::ID_GAUGE13 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT14 = wxNewId();
const long sixstatusFrame::ID_GAUGE14 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT15 = wxNewId();
const long sixstatusFrame::ID_GAUGE15 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT16 = wxNewId();
const long sixstatusFrame::ID_GAUGE16 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT17 = wxNewId();
const long sixstatusFrame::ID_GAUGE17 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT18 = wxNewId();
const long sixstatusFrame::ID_GAUGE18 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT31 = wxNewId();
const long sixstatusFrame::ID_GAUGE31 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT32 = wxNewId();
const long sixstatusFrame::ID_GAUGE32 = wxNewId();
const long sixstatusFrame::ID_STATICTEXT33 = wxNewId();
const long sixstatusFrame::ID_GAUGE33 = wxNewId();
const long sixstatusFrame::idMenuQuit = wxNewId();
const long sixstatusFrame::idMenuAbout = wxNewId();
const long sixstatusFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(sixstatusFrame,wxFrame)
    //(*EventTable(sixstatusFrame)
    //*)
END_EVENT_TABLE()

int lstick_x = 0;
int lstick_y = 0;
int rstick_x = 0;
int rstick_y = 0;
int bup = 0;
int bdown = 0;
int bright = 0;
int bleft = 0;
int br1 = 0;
int br2 = 0;
int bl1 = 0;
int bl2 = 0;
int bcircle = 0;
int bsquare = 0;
int bcross = 0;
int btriangle = 0;
int br3 = 0;
int bl3 = 0;
int bstart = 0;
int bselect = 0;
int bps = 0;

void read_status(void)
{
    string s;
    int skip;

    while(cin && getline(cin, s))
    {
        //cout << s << endl;
        stringstream parser(s);

        while(parser >> s)
        {
            if(s == "axis")
            {
                break;
            }
        }

        parser.ignore(2);
        parser >> lstick_x;
        parser.ignore(1);
        parser >> lstick_y;
        parser.ignore(3);
        parser >> rstick_x;
        parser.ignore(1);
        parser >> rstick_y;
        parser.ignore(7);
        parser >> skip;
        parser.ignore(1);
        parser >> skip;
        parser.ignore(1);
        parser >> skip;
        parser.ignore(1);
        parser >> skip;
        parser.ignore(2);

        bup = 0;
        bdown = 0;
        bright = 0;
        bleft = 0;
        br1 = 0;
        br2 = 0;
        bl1 = 0;
        bl2 = 0;
        bcircle = 0;
        bsquare = 0;
        bcross = 0;
        btriangle = 0;
        br3 = 0;
        bl3 = 0;
        bstart = 0;
        bselect = 0;
        bps = 0;

        while(parser >> s)
        {
            //cout << s << endl;
            if(!s.compare(0, 3, "up-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> bup;
            }
            else if(!s.compare(0, 5, "down-"))
            {
                string test (s, 5, 2);
                stringstream converter(test);
                converter >> hex >> bdown;
            }
            else if(!s.compare(0, 6, "right-"))
            {
                string test (s, 6, 2);
                stringstream converter(test);
                converter >> hex >> bright;
            }
            else if(!s.compare(0, 5, "left-"))
            {
                string test (s, 5, 2);
                stringstream converter(test);
                converter >> hex >> bleft;
            }
            else if(!s.compare(0, 3, "r1-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> br1;
            }
            else if(!s.compare(0, 3, "r2-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> br2;
            }
            else if(!s.compare(0, 3, "l1-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> bl1;
            }
            else if(!s.compare(0, 3, "l2-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> bl2;
            }
            else if(!s.compare(0, 3, "r1-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> br1;
            }
            else if(!s.compare(0, 3, "r2-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> br2;
            }
            else if(!s.compare(0, 7, "circle-"))
            {
                string test (s, 7, 2);
                stringstream converter(test);
                converter >> hex >> bcircle;
            }
            else if(!s.compare(0, 7, "square-"))
            {
                string test (s, 7, 2);
                stringstream converter(test);
                converter >> hex >> bsquare;
            }
            else if(!s.compare(0, 6, "cross-"))
            {
                string test (s, 6, 2);
                stringstream converter(test);
                converter >> hex >> bcross;
            }
            else if(!s.compare(0, 9, "triangle-"))
            {
                string test (s, 9, 2);
                stringstream converter(test);
                converter >> hex >> btriangle;
            }
            else if(!s.compare(0, 3, "r3-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> br3;
            }
            else if(!s.compare(0, 3, "l3-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> bl3;
            }
            else if(!s.compare(0, 6, "start-"))
            {
                string test (s, 6, 2);
                stringstream converter(test);
                converter >> hex >> bstart;
            }
            else if(!s.compare(0, 7, "select-"))
            {
                string test (s, 7, 2);
                stringstream converter(test);
                converter >> hex >> bselect;
            }
            else if(!s.compare(0, 3, "ps-"))
            {
                string test (s, 3, 2);
                stringstream converter(test);
                converter >> hex >> bps;
            }
        }
        //cout << lstick_x << " " << lstick_y << " " << rstick_x << " " << rstick_y << endl;
    }
}

sixstatusFrame::sixstatusFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(sixstatusFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxGridSizer* GridSizer1;
    wxMenuBar* MenuBar1;
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
    GridSizer1 = new wxGridSizer(10, 4, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Left Stick x"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge1 = new wxGauge(this, ID_GAUGE1, 255, wxDefaultPosition, wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    Gauge1->SetValue(127);
    GridSizer1->Add(Gauge1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Left Stick y"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge2 = new wxGauge(this, ID_GAUGE2, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE2"));
    Gauge2->SetValue(127);
    GridSizer1->Add(Gauge2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Right Stick x"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge3 = new wxGauge(this, ID_GAUGE3, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE3"));
    Gauge3->SetValue(127);
    GridSizer1->Add(Gauge3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Right Stick y"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    GridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge4 = new wxGauge(this, ID_GAUGE4, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE4"));
    Gauge4->SetValue(127);
    GridSizer1->Add(Gauge4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Up"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    GridSizer1->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge5 = new wxGauge(this, ID_GAUGE5, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE5"));
    GridSizer1->Add(Gauge5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Down"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridSizer1->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge6 = new wxGauge(this, ID_GAUGE6, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE6"));
    GridSizer1->Add(Gauge6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Right"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridSizer1->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge7 = new wxGauge(this, ID_GAUGE7, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE7"));
    GridSizer1->Add(Gauge7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Left"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridSizer1->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge8 = new wxGauge(this, ID_GAUGE8, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE8"));
    GridSizer1->Add(Gauge8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("R1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    GridSizer1->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge9 = new wxGauge(this, ID_GAUGE9, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE9"));
    GridSizer1->Add(Gauge9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("R2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    GridSizer1->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge10 = new wxGauge(this, ID_GAUGE10, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE10"));
    GridSizer1->Add(Gauge10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("L1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    GridSizer1->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge11 = new wxGauge(this, ID_GAUGE11, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE11"));
    GridSizer1->Add(Gauge11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("L2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    GridSizer1->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge12 = new wxGauge(this, ID_GAUGE12, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE12"));
    GridSizer1->Add(Gauge12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Circle"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    GridSizer1->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge13 = new wxGauge(this, ID_GAUGE13, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE13"));
    GridSizer1->Add(Gauge13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Square"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    GridSizer1->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge14 = new wxGauge(this, ID_GAUGE14, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE14"));
    GridSizer1->Add(Gauge14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Cross"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    GridSizer1->Add(StaticText15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge15 = new wxGauge(this, ID_GAUGE15, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE15"));
    GridSizer1->Add(Gauge15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Triangle"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    GridSizer1->Add(StaticText16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge16 = new wxGauge(this, ID_GAUGE16, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE16"));
    GridSizer1->Add(Gauge16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("R1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    GridSizer1->Add(StaticText17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge17 = new wxGauge(this, ID_GAUGE17, 255, wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_GAUGE17"));
    GridSizer1->Add(Gauge17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("R2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    GridSizer1->Add(StaticText18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge18 = new wxGauge(this, ID_GAUGE18, 255, wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_GAUGE18"));
    GridSizer1->Add(Gauge18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText31 = new wxStaticText(this, ID_STATICTEXT31, _("Start"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
    GridSizer1->Add(StaticText31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge31 = new wxGauge(this, ID_GAUGE31, 255, wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_GAUGE31"));
    GridSizer1->Add(Gauge31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText32 = new wxStaticText(this, ID_STATICTEXT32, _("Select"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT32"));
    GridSizer1->Add(StaticText32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge32 = new wxGauge(this, ID_GAUGE32, 255, wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_GAUGE32"));
    GridSizer1->Add(Gauge32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText33 = new wxStaticText(this, ID_STATICTEXT33, _("PS"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT33"));
    GridSizer1->Add(StaticText33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge33 = new wxGauge(this, ID_GAUGE33, 255, wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_GAUGE33"));
    GridSizer1->Add(Gauge33, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(GridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sixstatusFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sixstatusFrame::OnAbout);
    //*)

    monTimer.SetOwner(this, 1);
    monTimer.Start(10);

    Connect(1, wxEVT_TIMER,(wxObjectEventFunction)&sixstatusFrame::OnTimer);

    pthread_t thread;
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_create( &thread, &thread_attr, (void* (*)(void*))read_status, NULL);
}

sixstatusFrame::~sixstatusFrame()
{
    //(*Destroy(sixstatusFrame)
    //*)
}

void sixstatusFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void sixstatusFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

static void clamp(wxGauge* Gauge, int val, wxStaticText* Text)
{
    int high = Gauge->GetRange();
    if(val < 0)
    {
        Gauge->SetValue(0);
        Text->SetForegroundColour( wxColour(255, 0, 0) );
    }
    else if(val > high)
    {
        Gauge->SetValue(high);
        Text->SetForegroundColour( wxColour(255, 0, 0) );
    }
    else
    {
        Gauge->SetValue(val);
        Text->SetForegroundColour( wxColour(0, 0, 0) );
    }
}

void sixstatusFrame::OnTimer(wxTimerEvent& evt)
{
    clamp(Gauge1, 127+lstick_x, StaticText1);
    clamp(Gauge2, 127-lstick_y, StaticText4);
    clamp(Gauge3, 127+rstick_x, StaticText3);
    clamp(Gauge4, 127-rstick_y, StaticText2);
    clamp(Gauge5, bup, StaticText5);
    clamp(Gauge6, bdown, StaticText6);
    clamp(Gauge7, bright, StaticText7);
    clamp(Gauge8, bleft, StaticText8);
    clamp(Gauge9, br1, StaticText9);
    clamp(Gauge10, br2, StaticText10);
    clamp(Gauge11, bl1, StaticText11);
    clamp(Gauge12, bl2, StaticText12);
    clamp(Gauge13, bcircle, StaticText13);
    clamp(Gauge14, bsquare, StaticText14);
    clamp(Gauge15, bcross, StaticText15);
    clamp(Gauge16, btriangle, StaticText16);
    clamp(Gauge17, br3, StaticText17);
    clamp(Gauge18, bl3, StaticText18);
    clamp(Gauge31, bstart, StaticText31);
    clamp(Gauge32, bselect, StaticText32);
    clamp(Gauge33, bps, StaticText33);

    //cout << 127+lstick_x << " " << 127-lstick_y << " " << 127+rstick_x << " " << 127-rstick_y << endl;

    if(!cin)
    {
        cout << "no more cin: exiting" << endl;
        exit(-1);
    }
}
