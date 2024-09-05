
function Component()
{
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/analogclock.exe", "@StartMenuDir@/ADHDClock.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll",
            "iconId=2", "description=Open ADHDClock");
        // Run the VC14 installer
        component.addElevatedOperation("Execute", "{0,3010}", "@TargetDir@\\vcredist.exe", "/install /quiet /norestart");
    }
}

Component.prototype.installationFinishedPageIsShown = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "ReadMeCheckBoxForm", QInstaller.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var checkboxForm = component.userInterface( "ReadMeCheckBoxForm" );
            if (checkboxForm && checkboxForm.readMeCheckBox.checked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/analogclock.exe");
            }
        }
    } catch(e) {
        console.log(e);
    }
}
