/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Author: Devansh Jain (190100044)
 * Lab 5 - Question 1c - Hidden Terminal (No contention)
 */

#include "ns3/boolean.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/on-off-helper.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/string.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

// Run single 8 seconds experiment
void experiment(bool enableCtsRts, std::string wifiManager) {

  UintegerValue ctsThr =
      (enableCtsRts ? UintegerValue(100) : UintegerValue(10000));
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  ////////////////////////////////////////////////////////////////////////////////////
  // Nodes n0, n1, n2
  NodeContainer nodes;
  nodes.Create(3);
  for (uint8_t i = 0; i < 3; ++i) {
    nodes.Get(i)->AggregateObject(
        CreateObject<ConstantPositionMobilityModel>());
  }
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  // Create propagation loss matrix
  Ptr<MatrixPropagationLossModel> lossModel =
      CreateObject<MatrixPropagationLossModel>();
  lossModel->SetDefaultLoss(200); // Set default loss to 200 dB - No coverage
  // set symmetric loss n0 <-> n1, n0 <-> n2 to 50 dB, good coverage
  lossModel->SetLoss(nodes.Get(0)->GetObject<MobilityModel>(),
                     nodes.Get(1)->GetObject<MobilityModel>(), 50);
  lossModel->SetLoss(nodes.Get(0)->GetObject<MobilityModel>(),
                     nodes.Get(2)->GetObject<MobilityModel>(), 50);
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  // Create a YANS Wifi Channel
  Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
  wifiChannel->SetPropagationLossModel(lossModel);
  wifiChannel->SetPropagationDelayModel(
      CreateObject<ConstantSpeedPropagationDelayModel>());

  // Declare a wifi helper
  WifiHelper wifi;
  wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
                               StringValue("OfdmRate54Mbps"));

  // Create the wifi Phy helper
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
  wifiPhy.SetChannel(wifiChannel);
  wifiPhy.EnablePcap(enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node",
                     nodes);

  // Declare a WifiMac helper
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac"); // use ad-hoc MAC

  // Create the actual devices
  NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

  // Do the usual routine for Internet stack installation
  InternetStackHelper internet;
  // Install it on all the nodes
  internet.Install(nodes);

  // Declare the address helper
  Ipv4AddressHelper ipv4;
  ipv4.SetBase("10.0.0.0", "255.0.0.0");
  ipv4.Assign(devices); // Assign IP addresses to device interfaces
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  // Declare the application container
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;          // port for the CBR stream
  uint32_t payloadSize = 2200;       // Transport layer payload size in bytes.
  std::string dataRate = "54.0Mbps"; // Only this should be manipulated

  // OnOff source helpers - (n1) to n0
  OnOffHelper onOffHelper("ns3::UdpSocketFactory",
                          InetSocketAddress(Ipv4Address("10.0.0.1"), cbrPort));

  // OnOff source can be set to just generate packets at some data rate)
  onOffHelper.SetConstantRate(DataRate(dataRate), payloadSize);

  // Flow 1: Node n1 -> Node n0
  onOffHelper.SetAttribute("StartTime",
                           TimeValue(Seconds((double)101 / 100.0)));
  cbrApps.Add(onOffHelper.Install(nodes.Get(1)));
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  // UDP pingers due to lack of Address Resolution Protocol
  ApplicationContainer pingApps;
  uint16_t echoPort = 9;

  UdpEchoClientHelper echoClientHelper(Ipv4Address("10.0.0.1"), echoPort);
  echoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
  echoClientHelper.SetAttribute("Interval", TimeValue(Seconds(0.1)));
  echoClientHelper.SetAttribute("PacketSize", UintegerValue(10));

  echoClientHelper.SetAttribute("StartTime",
                                TimeValue(Seconds((double)1 / 1000)));
  pingApps.Add(echoClientHelper.Install(nodes.Get(1)));
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  // Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  // Run simulation for 8 seconds
  Simulator::Stop(Seconds(8));
  Simulator::Run();

  // Print per flow statistics
  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
  double totalTput = 0.0;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i =
           stats.begin();
       i != stats.end(); ++i) {
    // first FlowId is for ECHO app, we don't want to display them
    //
    // Duration for throughput measurement is 7.0 seconds, since
    //   StartTime of the OnOffApplication is at about 1 second
    // and
    //   Simulator::Stops at "second 8".
    if (i->first > 1) {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
      std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> "
                << t.destinationAddress << ")\n";
      std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
      std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
      std::cout << "  TxOffered:  "
                << i->second.txBytes * 8.0 / 7.0 / 1000 / 1000 << " Mbps\n";
      std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      std::cout << "  Throughput: "
                << i->second.rxBytes * 8.0 / 7.0 / 1000 / 1000 << " Mbps\n";
      totalTput += i->second.rxBytes * 8.0 / 7.0 / 1000 / 1000;
    }
  }
  std::cout << "Total channel throughput = " << totalTput << " Mbps"
            << std::endl;
  // Cleanup
  Simulator::Destroy();
  ////////////////////////////////////////////////////////////////////////////////////
}

int main(int argc, char **argv) {
  // Ignore this command line setup

  std::string wifiManager("Arf");
  CommandLine cmd;
  cmd.AddValue("wifiManager",
               "Set wifi rate manager (Aarf, Aarfcd, Amrr, Arf, Cara, Ideal, "
               "Minstrel, Onoe, Rraa)",
               wifiManager);
  cmd.Parse(argc, argv);
  //**Upto here

  std::cout
      << "Hidden station experiment (no contention) with RTS/CTS disabled:\n";
  experiment(false, wifiManager);
  std::cout << "------------------------------------------------\n";
  std::cout
      << "Hidden station experiment (no contention) with RTS/CTS enabled:\n";
  experiment(true, wifiManager);

  return 0;
}
