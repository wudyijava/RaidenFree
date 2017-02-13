package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;

public class Channel {

	static private ChannelInterface mInstance = null;
    static private boolean mFlagFlash = false;
    static private boolean mFlagStart = false;
    static private Activity mActivity = null;

	static public String getMainChannel()
	{
		return "default";
	}
	
	static public String getSubChannel()
	{
		return "";
	}

	static public ChannelInterface createInstance(Context context)
	{		
	    if(mInstance == null)
	    {
	        mInstance = new ChannelNone();
	    }
		
		return mInstance;
	}
	
	//初始化，在闪屏activity初始化的时候执行。如果没有闪屏，则不会调用
	static public void flash(Activity activity)
	{
		mActivity = activity;
        createInstance(mActivity);

	    if(!mFlagFlash)
	    {
	        mInstance.flash(mActivity);
	        mFlagFlash = true;
	    }

	}
	
	//在主activity初始化的时候必须第一个执行
	static public void setActivity(Activity activity)
	{
		mActivity = activity;
	}
	
    //初始化，在游戏activity初始化的时候执行
    static public boolean start()
    {
        createInstance(mActivity);
        flash(mActivity);

        if(!mFlagStart)
        {
            mFlagStart = true;
            return mInstance.start(mActivity);
        }
        
        return false;
    }

    //暂停接口
	static public boolean pause()
	{	    
		return mInstance.pause();
	}

	//退出接口
	static public boolean exit()
	{
		return mInstance.exit();
	}

	//每个activity界面的onResume函数中调用
	static public void onResume()
	{
		mInstance.onResume();
	}
	
	//每个activity界面的onPause函数中调用
	static public void onPause()
	{
		mInstance.onPause();
	}

}
