package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

public class BillingSimulate extends BillingInterface {

	private Activity mActivity;

	@Override
	public void loadLibrary(Context context) {

	}

	@Override
	public void initialize(Activity activity) {

		mActivity = activity;

	}

	@Override
	public boolean isMusicEnabled() {
		
		return true;
	}

	@Override
	public void moreGame() {
		

	}

	@Override
	public void pay(final String billingIndex, final String code, final String name, final int price, final String desc) {
		//弹出对话框，用来模拟付费成功、失败、取消
		final AlertDialog.Builder builder = new AlertDialog.Builder(mActivity)
				.setTitle("模拟支付  ID：" + billingIndex)
				.setPositiveButton("成功", new OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						
						Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
				               @Override
				               public void run() {
				            	   // 游戏业务收到付费结果后的处理逻辑
				            	   NativeInterface.payResult(billingIndex, Billing.ResultSuccess);
				               }
				           });
					}
				}).setNegativeButton("失败", new OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {

						Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
				               @Override
				               public void run() {
				            	   // 游戏业务收到付费结果后的处理逻辑
				            	   NativeInterface.payResult(billingIndex, Billing.ResultFailed);
				               }
				           });
					}
				});

		mActivity.runOnUiThread(new Runnable() {  
            public void run() {  
                builder.create().show();  
            }  
        });  
	}

	@Override
	public boolean exit() {

		return false;

	}

}
