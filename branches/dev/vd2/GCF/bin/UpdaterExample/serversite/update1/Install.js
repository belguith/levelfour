function DoInstall()
{
    System.Progress(0, "Installing SnapshotCapturer component");
    FileSystem.Copy("Update1.dll", "${APPDIR}/Update1.dll");

    System.Progress(50, "Configuring SnapshotCapturer component");
    FileSystem.Copy("SnapshotCapturerComponent.xml", "${APPDIR}/guixml/SnapshotCapturerComponent.xml");

    System.Progress(95, "Registering the update");
    System.RegisterComponent("SnapshotCapturerComponent", "Update1");

    System.Progress(100, "Finished installing update to SnapshotCapturer component");
    
    return true;
}
