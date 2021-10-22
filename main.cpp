#include "CApplication.h"

int main(int argc, char **argv)
{
    CApplication app;
    app.Run(GetModuleHandle(nullptr));
}
