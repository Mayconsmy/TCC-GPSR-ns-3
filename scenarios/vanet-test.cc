#include "ns3/aodv-helper.h"
#include "ns3/olsr-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gpsr-helper.h"
#include "ns3/log.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("GpsrRoutingProtocol", LOG_LEVEL_INFO);

  std::string routingProt = "GPSR";
  CommandLine cmd (__FILE__);
  cmd.AddValue ("routing", "Protocolo de Roteamento (GPSR, AODV, OLSR)", routingProt);
  cmd.Parse (argc, argv);

  uint32_t numNodes = 300;
  double simulationTime = 100.0;

  NodeContainer vehicles;
  vehicles.Create (numNodes);

  // 1. Camada Física (Antena estendida para testes)
  YansWifiPhyHelper wifiPhy;
  wifiPhy.Set ("TxPowerStart", DoubleValue (33.0));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (33.0));

  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");

  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211p);
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, vehicles);

  // 2. Mobilidade do SUMO
  Ns2MobilityHelper ns2 = Ns2MobilityHelper ("trace.tcl");
  ns2.Install ();

  // 3. Instalacao do Roteamento Dinamico
  InternetStackHelper stack;
  if (routingProt == "GPSR") {
      GpsrHelper gpsr;
      stack.SetRoutingHelper (gpsr);
  } else if (routingProt == "AODV") {
      AodvHelper aodv;
      stack.SetRoutingHelper (aodv);
  } else if (routingProt == "OLSR") {
      OlsrHelper olsr;
      stack.SetRoutingHelper (olsr);
  } else {
      NS_FATAL_ERROR ("Protocolo invalido! Escolha GPSR, AODV ou OLSR.");
  }
  stack.Install (vehicles);

  // CORREÇÃO: IP Base ajustado para 10.1.0.0 para casar com a máscara /16
  Ipv4AddressHelper address;
  address.SetBase ("10.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // 4. Trafego UDP
  // CORREÇÃO: Escolhemos carros que acabaram de "nascer" no SUMO perto do segundo 20.
  // Isso garante que eles estão vivos e no meio do trânsito na hora do teste.
  uint32_t serverNode = 29;
  uint32_t clientNode = 25;

  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (vehicles.Get (serverNode));
  serverApps.Start (Seconds (20.0));
  serverApps.Stop (Seconds (simulationTime));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (serverNode), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (vehicles.Get (clientNode));
  // O cliente dispara quando o trânsito começa a ficar denso
  clientApps.Start (Seconds (30.0));
  clientApps.Stop (Seconds (simulationTime));

  // 5. Metricas
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  NS_LOG_UNCOND ("Iniciando simulacao VANET. Protocolo em uso: " << routingProt);
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();

  monitor->SerializeToXmlFile ("gpsr-sumo-results.xml", true, true);

  Simulator::Destroy ();
  NS_LOG_UNCOND ("Simulacao concluida! Metricas salvas em 'gpsr-sumo-results.xml'.");

  return 0;
}
