package org.cocos2dx.cpp;

import android.content.Context;
import android.telephony.TelephonyManager;


/**
 * @author thorqq
 * 运营商帮助类
 */
public class Carrier {
	/**
	 * 未知运营商
	 */
	static public final int Unknown = -1;

	/**
	 * 中国移动
	 */
	static public final int ChinaMobile = 10; 
	
	/**
	 * 中国电信
	 */
	static public final int ChinaTelcom = 20;
	
	/**
	 * 中国联通
	 */
	static public final int ChinaUnicom = 30;
	
	/**
	 * 无运营商，不调用计费
	 */
	static public final int Simulation = 99;
	
	static private int mCarrier = ChinaMobile;
	
    static public int getCarrier()
    {
        return mCarrier;
    }

    /**
	 * @param context
	 * @return 当前用户号码归属的运营商
	 */
	static public int getCarrier(Context context)
	{
		TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
		String imsi = tm.getSubscriberId();

		if(imsi != null)
		{
			if(imsi.startsWith("46000") || imsi.startsWith("46002"))
			{
				//因为移动网络编号46000下的IMSI已经用完，所以虚拟了一个46002编号，134/159号段使用了此编号 //中国移动
			    mCarrier = ChinaMobile;
	
			}else if(imsi.startsWith("46001"))
			{
				//中国联通
			    mCarrier = ChinaUnicom;
	
			}else if(imsi.startsWith("46003"))
			{
				//中国电信
			    mCarrier = ChinaTelcom;
			}
			else
			{
			    mCarrier = ChinaMobile;
			}
		}
		else
		{
	        mCarrier = ChinaMobile;
		}
		
		return mCarrier;
	}
}
