#include "AppWindow.h"

int main() {

	/*AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*) AppWindow::getInstance();
	runningApp->createGraphicsWindow();

	while (runningApp->isRun()) {
		runningApp->broadcast();
	}*/
	AppWindow app;
	if (app.init())
	{
		while (app.isRun()) {
			app.broadcast();
		}
	}
	return 0;
}