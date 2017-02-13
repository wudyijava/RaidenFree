package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;

public class Billing {

	static public final int ResultSuccess = 0;
	static public final int ResultFailed  = 1;
	static public final int ResultCancel  = 2;
	static public final int ResultTimeout = 3;
	
	//注意，这里的包类型只能是2个字节！！！
	static public final int BillingTypeSimulate       = 99;
		
	
	static private BillingInterface mInstance = null;
	static private int mBillingType = BillingTypeSimulate;
	

	static public BillingInterface createInstance(Context context)
	{
		mBillingType = BillingTypeSimulate;
		mInstance = new BillingSimulate();
				
		return mInstance;
	}
	
	static public void destoryInstance()
	{
		
	}
	
	static public int getBillingType()
	{
	    return mBillingType;
	}
	
	static public void loadLibrary(Context context)
	{
		mInstance.loadLibrary(context);
	}
	
	static public void initializeWithActivity(Activity activity) {
		mInstance.initialize(activity);
	}

	static public boolean isMusicEnabled() {
		return mInstance.isMusicEnabled();
	}

	static public void moreGame() {
		mInstance.moreGame();
	}

	static public void pay(String billingIndex, String code, String name, int price, String desc) {
		mInstance.pay(billingIndex, code, name, price, desc);
	}

	static public boolean exit() {
		return mInstance.exit();
	}

}
