package org.cocos2dx.cpp;

public class NativeInterface {
	
	//确认退出游戏
	static public native void exitConfirm();
	
	//取消退出游戏
	static public native void exitCancel();
	
	//处理支付结果
	static public native void payResult(String billingIndex, int resultCode);

}
