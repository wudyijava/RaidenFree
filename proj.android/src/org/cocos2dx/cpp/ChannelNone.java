package org.cocos2dx.cpp;

import android.app.Activity;

public class ChannelNone implements ChannelInterface {
	
	@Override
	public void flash(Activity activity) {		

	}

    @Override
    public boolean start(Activity activity) {     
    	
    	return false;
    }

    @Override
	public boolean pause() {

		return false;
	}

	@Override
	public boolean exit() {

		return false;
	}

	@Override
	public void onResume() {

	}

	@Override
	public void onPause() {

	}
}