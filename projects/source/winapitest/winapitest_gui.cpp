//=============================================================================================================
#include "winapitest.h"

void WinapiTest::InitializeGUI()
{
	// load common controls
	INITCOMMONCONTROLSEX iccs;

    iccs.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccs.dwICC = ICC_STANDARD_CLASSES|ICC_BAR_CLASSES|ICC_UPDOWN_CLASS;

    InitCommonControlsEx(&iccs);

	// initialize controls
	picturebox1.Location.x = 10;
	picturebox1.Location.y = 10;

	int mh = GetSystemMetrics(SM_CYMENU);
	int sh = 23; // zipp

	long width = DisplayMode.Width + 20 + 226;
	long height = DisplayMode.Height + 20 + mh + sh;

	size_t id1 = menustrip1.AddMenu("&File");
	size_t id2 = menustrip1.AddMenu("&Help");

	menustrip1.AddItem(id1, "&Open");
	menustrip1.AddSeparator(id1);
	menustrip1.AddItem(id1, "&Exit");
	menustrip1.AddItem(id2, "&About");

	statusstrip1.AddDivider(width - 324);
	statusstrip1.AddDivider(width - 142);
	statusstrip1.Text = "Click File/Open to open an image";

	// create window
	Application.Initialize(width, height, false);
	HWND hwnd = Application.GetWindowHandle();

	openfile1.Filter = "Image files (*.bmp;*.jpg;*.png;*.tga;*.dds)|*.bmp;*.jpg;*.png;*.tga;*.dds|All Files (*.*)|*.*";
	openfile1.InitialDirectory = Application.ExecutablePath + "..\\media\\textures";

	// resize to window
	picturebox1.Size.x = (int)width - 20 - 226;
	picturebox1.Size.y = (int)height - 20 - mh - sh;
	picturebox1.DelegateInput = true;

	groupbox1.Location.x = picturebox1.Location.x + picturebox1.Size.x + 6;
	groupbox1.Location.y = picturebox1.Location.y;
	groupbox1.Size.x = 220;
	groupbox1.Size.y = 100;
	groupbox1.Text = "Bloom intensity";
	{
		trackbar1.Location.x = groupbox1.Location.x + 6;
		trackbar1.Location.y = groupbox1.Location.y + 20;
		trackbar1.Size.x = 152;
		trackbar1.Size.y = 45;
		trackbar1.TickFreq = 10;
		trackbar1.DelegateInput = true;
		trackbar1.DrawFocusRect = false;

		button1.Text = "Toggle";
		button1.Size.x = 101;
		button1.Size.y = 23;
		button1.Location.x = groupbox1.Location.x + groupbox1.Size.x - button1.Size.x - 6;
		button1.Location.y = groupbox1.Location.y + groupbox1.Size.y - button1.Size.y - 6;
		button1.DelegateInput = true;
		button1.DrawFocusRect = false;

		updown1.Size.x = 48;
		updown1.Size.y = 20;
		updown1.Location.x = groupbox1.Location.x + 164;
		updown1.Location.y = groupbox1.Location.y + 19;
		updown1.MinValue = 0;
		updown1.MaxValue = 1;
		updown1.Increment = 0.01;
		updown1.Precision = 2;
	}
	
	label1.Text = "Bloom toggled off";
	label1.Size.x = 170;
	label1.Size.y = 17;
	label1.Location.x = width - 324 + 6;
	label1.Location.y = height - mh - sh + 4 + 2;

	combo1.Location.x = groupbox1.Location.x;
	combo1.Location.y = groupbox1.Location.y + groupbox1.Size.y + 16;
	combo1.Size.x = 150;
	combo1.Size.y = 20;
	combo1.DropDownStyle = DummyFramework::dds_dropdownlist;

	// init
	menustrip1.Initialize(hwnd);
	picturebox1.Initialize(hwnd);
	statusstrip1.Initialize(hwnd);
	groupbox1.Initialize(hwnd);
	updown1.Initialize(hwnd);
	trackbar1.Initialize(hwnd);
	button1.Initialize(hwnd);
	label1.Initialize(hwnd);
	combo1.Initialize(hwnd);

	trackbar1.SetRange(0, 100);
	trackbar1.SetPos(55);
	updown1.SetValue(0.55f);

	combo1.AddString("Apple");
	combo1.AddString("Lemon");
	combo1.AddString("\\Banane");
	combo1.AddString("Kiwi");
	combo1.AddString("\\Mango");
	combo1.AddString("\\Orange");

	combo1.SetCurrentItem(1);

#ifdef _DEBUG
	DummyFramework::CWindow::CheckError();
	std::cout << "\n";
#endif
}
//=============================================================================================================

