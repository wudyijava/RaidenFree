package org.cocos2dx.cpp;

import android.app.Activity;

public interface ChannelInterface {
	
	//初始化，在activity初始化的时候执行
	public void flash(Activity activity);
	    
    //初始化，在游戏activity初始化的时候执行
    public boolean start(Activity activity);

    //暂停接口
	public boolean pause();

	//退出接口
	public boolean exit();

	//每个activity界面的onResume函数中调用
	public void onResume();
	
	//每个activity界面的onPause函数中调用
	public void onPause();
	

}
