package org.openhome.net.test;

import java.net.Inet4Address;

import ohnet.NetworkAdapter;

import org.openhome.net.controlpoint.CpDeviceDv;
import org.openhome.net.core.InitParams;
import org.openhome.net.core.Library;
import org.openhome.net.core.NetworkAdapterList;

public class TestCpDeviceDv
{
	
	public static void main(String[] args)
	{
		InitParams initParams = new InitParams();
		initParams.setMsearchTimeSecs(1);
		initParams.setUseLoopbackNetworkAdapter();
		Library lib = new Library();
		lib.initialise(initParams);
		NetworkAdapterList subnetList = lib.subnetListCreate();
		NetworkAdapter nif = subnetList.getSubnet(0);
		Inet4Address subnet = nif.getSubnet();
		subnetList.destroy();
		lib.startCombined(subnet);
		
		System.out.println("TestCpDeviceDvJava - starting");
		DeviceBasic device = new DeviceBasic();
		CpDeviceDv cpDevice = new CpDeviceDv(device.getDevice());
		TestBasicCp cp = new TestBasicCp(cpDevice);
		cp.testActions();
		cp.testSubscriptions();
		cpDevice.removeRef();
		device.dispose();
		System.out.println("TestCpDeviceDvJava - completed");
	}

}
