

class ServiceAddonServer : BApplication {
public:
								ServiceAddonServer(const char* signature);
	virtual						~ServiceAddonServer();
	virtual	void				ReadyToRun();
	virtual bool				QuitRequested();
	virtual void				MessageReceived(BMessage* message);

private:

};


int
main()
{
	new ServiceAddonServer("signature");
	be_app->Run();
	delete be_app;
	return 0;
}
