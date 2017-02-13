/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity
{
    static final String     LOG_TAG   = "raiden";
    static private Activity mActivity = null;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // 设置渠道sdk的activity
        Channel.setActivity(this);

        mActivity = this;

        // 计费sdk初始化
        Billing.initializeWithActivity(this);

    }

    public static void showCopyright()
    {
        mActivity.runOnUiThread(new Runnable()
        {
            public void run()
            {
                Toast.makeText(mActivity, "请使用正版软件!", Toast.LENGTH_LONG).show();
            }
        });
    }

    /**
     * @return 版本号
     */
    static public int getVersionCode()
    {
        try
        {
            // 获取软件版本号
            return getContext().getPackageManager().getPackageInfo(getContext().getPackageName(), 0).versionCode;

        }
        catch (NameNotFoundException e)
        {
            e.printStackTrace();
            return -1;

        }
        catch (Exception e)
        {
            e.printStackTrace();
            return -1;

        }

    }

    /**
     * @return 版本名称
     */
    static public String getVersionName()
    {
        try
        {
            // 获取软件版本号
            return getContext().getPackageManager().getPackageInfo(getContext().getPackageName(), 0).versionName;

        }
        catch (NameNotFoundException e)
        {
            e.printStackTrace();
            return "Unknown version";

        }
        catch (Exception e)
        {
            e.printStackTrace();
            return "Unknown version";

        }

    }

    @Override
    public void onResume()
    {

        Channel.onResume();

        super.onResume();
    }

    @Override
    public void onPause()
    {

        Channel.onPause();

        super.onPause();
    }

}
