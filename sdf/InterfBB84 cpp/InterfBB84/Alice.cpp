#include "Alice.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]

int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	//Application::SetCompatibleTextRenderingDefault(false);
	InterfBB84::Alice form;
	//InterfBB84::MyForm1 form2;
	Application::Run(%form);

	//textBox1->Text = "TextBox Test Control";
	//Application::Run(%form2);
	return 0;
}