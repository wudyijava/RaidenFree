package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;

public abstract class BillingInterface {
	
	//载入动态库
	public abstract void loadLibrary(Context context);

	//初始化，在activity初始化的时候执行
	public abstract void initialize(Activity activity);
	
	//进入游戏时必须使用这个方法进行判断音乐是否开关
	public abstract boolean isMusicEnabled();

	//显示更多游戏
	public abstract void moreGame();

	//支付接口 
	//billingIndex:计费点编号
	//name  产品名称
	//price 金额-分
	//desc  产品描述
	public abstract void pay(final String billingIndex, final String code, final String name, final int price, final String desc);

	//退出接口
	public abstract boolean exit();
}

