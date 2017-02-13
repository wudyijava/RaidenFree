package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;

public class MyApplication extends Application
{

    static private Context mContext;

    static public Context getContext()
    {
        return mContext;
    }

    @Override
    public void onCreate()
    {
        mContext = this;

        Carrier.getCarrier(this);

        Billing.createInstance(this).loadLibrary(this);

        Channel.createInstance(this);

        super.onCreate();
    }

}
