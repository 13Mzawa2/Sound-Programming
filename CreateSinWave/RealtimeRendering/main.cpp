//-------------------------------------------------------
//		XAudio2によるリアルタイムサウンド合成
//-------------------------------------------------------

#include "TestForm.h"

using namespace RealtimeRendering;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew TestForm());
	return 0;
}