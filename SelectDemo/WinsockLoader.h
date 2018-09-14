#pragma once
class WinsockLoader
{
public:
	static WinsockLoader& Get();
	bool InitLib();
	bool IsInitialized();
private:
	WinsockLoader();
	~WinsockLoader();
private:
	static WinsockLoader socLoader_;
	bool initialized_ = false;
};

