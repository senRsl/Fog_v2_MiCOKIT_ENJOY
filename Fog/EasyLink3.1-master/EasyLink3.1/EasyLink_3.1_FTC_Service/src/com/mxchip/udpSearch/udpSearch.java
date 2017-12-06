package com.mxchip.udpSearch;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class udpSearch {

	// private int iReceivePort = 0; // 接收到消息的端口号
	// private static final int TIMEOUT = 1000; // 设置超时为100ms
	private String udpresult = null; // 服务器返回的消息
	// private String ip;
	private DatagramSocket socketSend, socketReceive = null;
	boolean bFindGateway = false;

	/**
	 * Send UDP broadcast
	 * 
	 * @param BroadcastData
	 * @param BroadcastPort
	 * @param BroadcastIP
	 * @param listener
	 */
	public void doUdpFind(final String BroadcastData, final int BroadcastPort,
			final String BroadcastIP, final udpSearch_Listener listener) {
		try {
			socketSend = socketReceive = new DatagramSocket();
			socketSend.setReuseAddress(true);
			socketReceive.setReuseAddress(true);
		} catch (SocketException e1) {
			e1.printStackTrace();
		}

		new Thread(new Runnable() {

			@Override
			public void run() {
				try {
					// 发送设置为广播
					socketSend.setBroadcast(true);
					// 设置发送的消息
					String str = BroadcastData;
					// 发送和接收的数据包
					byte[] dataSend = str.getBytes();

					// 发送地址
					InetAddress address = InetAddress.getByName(BroadcastIP);
					// 创建发送类型的数据报
					DatagramPacket sendPacket = new DatagramPacket(dataSend,
							dataSend.length, address, BroadcastPort);
					int i = 0;
					// 是否接收到数据

					while (!bFindGateway && i < 5) {
						i++;
						// 通过套接字发送数据
						socketSend.send(sendPacket);
						Thread.sleep(1000l);
					}
					if (null == udpresult)
						listener.onDeviceFound(null);
					bFindGateway = true;
					// 关闭套接字
					socketSend.close();
					socketReceive.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();

		new Thread(new Runnable() {

			@Override
			public void run() {
				byte[] dataReceive = new byte[1024];
				DatagramPacket packetReceive = new DatagramPacket(dataReceive,
						dataReceive.length);

				while (!bFindGateway) {
					dataReceive = new byte[1024];
					packetReceive = new DatagramPacket(dataReceive,
							dataReceive.length);
					// 通过套接字接收数据
					try {
						socketReceive.receive(packetReceive);

						// System.out.println("receive message is ok.");
						// 获得反馈ip地址
						// ip = packetReceive.getAddress().getHostAddress();
						// 服务器返回的数据
						udpresult = new String(packetReceive.getData(),
								packetReceive.getOffset(),
								packetReceive.getLength());
						// Log.i("", "data:" + udpresult);
						// Log.i("", "ip:" + ip);
						listener.onDeviceFound(udpresult.trim());
					} catch (IOException e) {
						// e.printStackTrace();
					}
				}

			}
		}).start();
	}

	public void stopUdpFind() {
		bFindGateway = true;
	}
}
