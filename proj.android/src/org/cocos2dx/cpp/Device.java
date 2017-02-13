package org.cocos2dx.cpp;

import android.app.Service;
import android.os.Vibrator;

class Device
{
	public static void vibrate(int milliseconds)
	{
		Vibrator vib = (Vibrator) MyApplication.getContext().getSystemService(Service.VIBRATOR_SERVICE);   
        vib.vibrate(milliseconds);   
	}
}