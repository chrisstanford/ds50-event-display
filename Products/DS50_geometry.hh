#ifndef PMT_positions_hh
#define PMT_positions_hh

#include <cmath>
#include <string>

namespace display{
  class PMT_positions {
  public:
    double GetPMTx() { return x; }
    double GetPMTy() { return y; }
    double GetPMTz() { return z; }

    double GetPMTtheta() {
      return acos(GetPMTz()/GetPMTr());
    }
    double GetPMTphi() {
      return atan(GetPMTy()/GetPMTx());
      /*
	if (detector=="lsv"||detector=="wt") {
	return atan(GetPMTy()/GetPMTx());
	} else if (detector=="tpc") {
	std::cout<<"Adding offset\n.";
	return atan(GetPMTy()/GetPMTx())
	+tpc_lsv_offset; // TPC-LSV azimuthal offset
	}*/
    }
    double GetPMTr() {
      return sqrt(GetPMTx()*GetPMTx()+
		  GetPMTy()*GetPMTy()+
		  GetPMTz()*GetPMTz());
    }
    double GetPMTrho() {
      return sqrt(GetPMTx()*GetPMTx()+
		  GetPMTy()*GetPMTy());
    }
    double GetPMTRotx() {
      if(detector=="lsv"||detector=="wt") {
	if (GetPMTx()>0) {
	  return GetPMTphi();
	} else {
	  return GetPMTphi()+pi;
	}
      } else if (detector=="tpc") {
	return 0;
      }
      return def;
    }
    double GetPMTRoty() {
      if(detector=="lsv") {
	return -GetPMTtheta();
      } else if (detector=="wt") {
	if (GetPMTz()<-4600) {
	  return pi;
	} else {
	  return -pi/2;
	}
      } else if (detector=="tpc") {
	if (GetPMTz()<0) {
	  return pi;
	} else {
	  return 0;
	}
      }
      return def;
    }
    double GetPMTRotz() {
      return 0;
    }

    PMT_positions(const std::string _detector, int _ch_id) : 
      detector(_detector), 
      ch_id(_ch_id),
      pi(3.1415926535897),
      def(1.e9),
      dx(1.e-9), 
      Dx(150.),
      Dy(Dx/sqrt(3.)),
      Dz(400),
      x(0),
      y(0),
      z(0),
      tpc_lsv_offset(27.2*(2.*pi/360))
    {	

      if (detector=="lsv"||detector=="wt") {  
	if(ch_id==0)   {x=1746.54822;	        y=-1070.286126;	z=-361.1882095;}
	if(ch_id==1)   {x=1306.640676;	y=1239.955703;	z=-1040;}
	if(ch_id==2)   {x=-918.6/*1101.855254*/;	y=-971.4/*757.2841352*/;	z=1593.372442;}
	if(ch_id==3)   {x=-1557.615676;	y=-1330.329464;	z=361.1882095;}
	if(ch_id==4)   {x=-1206.754926;	y=575.5925736;	z=-1593.372442;}
	if(ch_id==5)   {x=-1698.011074;	y=-601.2972577;	z=1040;}
	if(ch_id==6)   {x=-1795.779932;	y=141.3309457;	z=1040;}
	if(ch_id==7)   {x=-1306.640676;	y=-1239.955703;	z=1040;}
	if(ch_id==8)   {x=-1991.802668;	y=478.1895128;	z=361.1882095;}
	if(ch_id==9)   {x=1281.275584;	y=381.964841;	z=1593.372442;}
	if(ch_id==10)  {x=-2042.085592;	y=-160.7156216;	z=-361.1882095;}
	if(ch_id==11)  {x=-1991.802668;	y=478.1895128;	z=-361.1882095;}
	if(ch_id==12)  {x=-1281.275584;	y=-381.964841;	z=1593.372442;}
	if(ch_id==13)  {x=1535.888729;	y=941.1938234;	z=1040;}
	if(ch_id==14)  {x=2042.085592;	y=160.7156216;	z=361.1882095;}
	if(ch_id==15)  {x=711.4018981;	y=dx;	        z=1954.560651;}
	if(ch_id==16)  {x=503.0371063;	y=-503.0371063;	z=1954.560651;}
	if(ch_id==17)  {x=1557.615676;	y=1330.329464;	z=361.1882095;}
	if(ch_id==18)  {x=1991.802668;	y=-478.1895128;	z=361.1882095;}
	if(ch_id==19)  {x=1785.922188;	y=235.1211165;	z=1040;}
	if(ch_id==20)  {x=-160.7156216;	y=2042.085592;	z=-361.1882095;}
	if(ch_id==21)  {x=dx;		        y=-711.4018981;	z=1954.560651;}
	if(ch_id==22)  {x=1746.54822;	        y=-1070.286126;	z=361.1882095;}
	if(ch_id==23)  {x=def;	        y=def;		z=def;}
	if(ch_id==24)  {x=-1892.474951;	y=-783.8887912;	z=361.1882095;}
	if(ch_id==25)  {x=-309.8465363;	y=-1300.599626;	z=1593.372442;}
	if(ch_id==26)  {x=-1070.286126;	y=-1746.54822;	z=361.1882095;}
	if(ch_id==27)  {x=-478.1895128;	y=-1991.802668;	z=361.1882095;}
	if(ch_id==28)  {x=-783.8887912;	y=1892.474951;	z=-361.1882095;}
	if(ch_id==29)  {x=-689.340234;	y=-1664.214542;	z=1040;}
	if(ch_id==30)  {x=-775.4937766;	y=1625.856513;	z=-1040;}
	if(ch_id==31)  {x=47.15339663;	y=-1800.715568;	z=1040;}
	if(ch_id==32)  {x=1300.599626;	y=-309.8465363;	z=1593.372442;}
	if(ch_id==33)  {x=1096.581954;	y=-1429.093425;	z=-1040;}
	if(ch_id==34)  {x=783.8887912;	y=-1892.474951;	z=-361.1882095;}
	if(ch_id==35)  {x=160.7156216;	y=-2042.085592;	z=361.1882095;}
	if(ch_id==36)  {x=-104.899672;	y=1332.876709;	z=-1593.372442;}
	if(ch_id==37)  {x=1892.474951;	y=783.8887912;	z=361.1882095;}
	if(ch_id==38)  {x=1795.779932;	y=-141.3309457;	z=-1040;}
	if(ch_id==39)  {x=-1746.54822;	y=1070.286126;	z=361.1882095;}
	if(ch_id==40)  {x=1330.329464;	y=-1557.615676;	z=-361.1882095;}
	if(ch_id==41)  {x=382/*104.899672*/;		y=-1281/*1332.876709*/;	z=1593.372442;}
	if(ch_id==42)  {x=-1727.153476;	y=511.6061674;	z=-1040;}
	if(ch_id==43)  {x=-503.0371063;	y=-503.0371063;	z=1954.560651;}
	if(ch_id==44)  {x=971.4290477;	y=-918.6347845;	z=1593.372442;}
	if(ch_id==45)  {x=1369.744237;	y=-1169.872097;	z=1040;}
	if(ch_id==46)  {x=783.8887912;	y=-1892.474951;	z=361.1882095;}
	if(ch_id==47)  {x=575.5925736;	y=1206.754926;	z=-1593.372442;}
	if(ch_id==48)  {x=-381.964841;	y=1281.275584;	z=1593.372442;}
	if(ch_id==49)  {x=-711.4018981;	y=dx;   	z=1954.560651;}
	if(ch_id==50)  {x=1727.153476;	y=-511.6061674;	z=1040;}
	if(ch_id==51)  {x=1330.329464;	y=-1557.615676;	z=361.1882095;}
	if(ch_id==52)  {x=-160.7156216;	y=2042.085592;	z=361.1882095;}
	if(ch_id==53)  {x=-1096.581954;	y=1429.093425;	z=1040;}
	if(ch_id==54)  {x=-2042.085592;	y=-160.7156216;	z=361.1882095;}
	if(ch_id==55)  {x=775.4937766;	y=-1625.856513;	z=1040;}
	if(ch_id>55&&ch_id<64) {x=def; 	y=def;		z=def;}
	if(ch_id==64)  {x=1020.286155;	y=1484.525568;	z=1040;}
	if(ch_id==65)  {x=dx;			y=711.4018981;	z=1954.560651;}
	if(ch_id==66)  {x=309.8465363;	y=1300.599626;	z=1593.372442;}
	if(ch_id==67)  {x=478.1895128;	y=1991.802668;	z=361.1882095;}
	if(ch_id==68)  {x=503.0371063;	y=503.0371063;	z=1954.560651;}
	if(ch_id==69)  {x=711.4018981;	y=dx;	        z=-1954.560651;}
	if(ch_id==70)  {x=328.2668367;	y=1771.169355;	z=1040;}
	if(ch_id==71)  {x=1070.286126;	y=1746.54822;	z=361.1882095;}
	if(ch_id==72)  {x=420.5128002;	y=-1751.56187;	z=-1040;}
	if(ch_id==73)  {x=1070.286;	y=1746.548;	z=-361.1882095;}
	if(ch_id==74)  {x=757.2841352;	y=-1101.855254;	z=-1593.372442;}
	if(ch_id==75)  {x=1892.474951;	y=783.8887912;	z=-361.1882095;}
	if(ch_id==76)  {x=1991.802668;	y=-478.1895128;	z=-361.1882095;}
	if(ch_id==77)  {x=1583.042125;	y=-859.5217447;	z=-1040;}
	if(ch_id==78)  {x=1332.876709;	y=104.899672;	z=-1593.372442;}
	if(ch_id==79)  {x=def;	        y=def;	        z=def;}
	if(ch_id==80)  {x=-757.2841352;	y=1101.855254;	z=-1593.372442;}
	if(ch_id==81)  {x=-47.15339663;	y=1800.715568;	z=-1040;}
	if(ch_id==82)  {x=478.1895128;	y=1991.802668;	z=-361.1882095;}
	if(ch_id==83)  {x=689.340234; 	y=1664.214542;	z=-1040;}
	if(ch_id==84)  {x=1698.011074;	y=601.2972577;	z=-1040;}
	if(ch_id==85)  {x=1207/*1288.372209*/;	y=-575.5/*357.2972353*/;	z=-1593.372442;}
	if(ch_id==86)  {x=1101.855254;	y=757.2841352;	z=-1593.372442;}
	if(ch_id==87)  {x=1557.615676;	y=1330.329464;	z=-361.1882095;}
	if(ch_id==88)  {x=-503.0371063;	y=-503.0371063;	z=-1954.560651;}
	if(ch_id==89)  {x=-1330.329464;	y=1557.615676;	z=-361.1882095;}
	if(ch_id==90)  {x=2042.085592;	y=160.7156216;	z=-361.1882095;}
	if(ch_id==91)  {x=dx;	        	y=711.4018981;	z=-1954.560651;}
	if(ch_id==92)  {x=-1369.744237;	y=1169.872097;	z=-1040;}
	if(ch_id==93)  {x=-1746.54822;	y=1070.286126;	z=-361.1882095;}
	if(ch_id==94)  {x=503.0371063;	y=-503.0371063;	z=-1954.560651;}
	if(ch_id==95)  {x=-503.0371063;	y=503.0371063;	z=-1954.560651;}
	if(ch_id==96)  {x=503.0371063;	y=503.0371063;	z=-1954.560651;}
	if(ch_id==97)  {x=-1892.474951;	y=-783.8887912;	z=-361.1882095;}
	if(ch_id==98)  {x=-1332.876709;	y=-104.899672;	z=-1593.372442;}
	if(ch_id==99)  {x=-1785.922188;	y=-235.1211165;	z=-1040;}
	if(ch_id==100) {x=104.899672;		y=-1332.876709;	z=-1593.372442;}
	if(ch_id==101) {x=-1020.286155;	y=-1484.525568;	z=-1040;}
	if(ch_id==102) {x=-711.4018981;	y=dx;		z=-1954.560651;}
	if(ch_id==103) {x=-1535.888729;	y=-941.1938234;	z=-1040;}
	if(ch_id==104) {x=dx;			y=-711.4018981;	z=-1954.560651;}
	if(ch_id==105) {x=-575.5925736;	y=-1206.754926;	z=-1593.372442;}
	if(ch_id==106) {x=-1070.286126;	y=-1746.54822;	z=-361.1882095;}
	if(ch_id==107) {x=-328.2668367;	y=-1771.169355;	z=-1040;}
	if(ch_id==108) {x=160.7156216;	y=-2042.085592;	z=-361.1882095;}
	if(ch_id==109) {x=-1557.615676;	y=-1330.329464;	z=-361.1882095;}
	if(ch_id==110) {x=-478.1895128;	y=-1991.802668;	z=-361.1882095;}
	if(ch_id==111) {x=-1101.855254;	y=-757.2841352;	z=-1593.372442;}
	if(ch_id==112) {x=-783.8887912;	y=1892.474951;	z=361.1882095;}
	if(ch_id==113) {x=-1301/*1206.754926*/;	y=310/*575.5925736*/;	z=1593.372442;}
	if(ch_id==114) {x=-971.4290477;	y=918.6347845;	z=1593.372442;}
	if(ch_id==115) {x=-1330.329464;	y=1557.615676;	z=361.1882095;}
	if(ch_id==116) {x=-420.5128002;	y=1751.56187;	z=1040;}
	if(ch_id==117) {x=-503.0371063;	y=503.0371063;	z=1954.560651;}
	if(ch_id==118) {x=918.6347845;	y=971.4290477;	z=1593.372442;}
	if(ch_id==119) {x=-1583.042125;	y=859.5217447;	z=1040;}
	if(ch_id>119&&ch_id<128) {x=def;	y=def;		z=def;}
	if(ch_id==128) {x=-1414.21;		y=-1414.21;	z=-4608;}
	if(ch_id==129) {x=-3889.09;		y=-3889.09;	z=1642;}
	if(ch_id==130) {x=dx;			y=2000;		z=-4608;}
	if(ch_id==131) {x=dx;			y=4500;		z=-4608;}
	if(ch_id==132) {x=5500;		y=dx;		z=392;}
	if(ch_id==133) {x=-2298.1;		y=2298.1;	z=-4608;}
	if(ch_id==134) {x=2298.1;		y=2298.1;	z=-4608;}
	if(ch_id==135) {x=5500;		y=dx;		z=-3358;}
	if(ch_id==136) {x=3889.09;		y=-3889.09;	z=-858;}
	if(ch_id==137) {x=5500;		y=dx;		z=4142;}
	if(ch_id==138) {x=-3889.09;		y=-3889.09;	z=2892;}
	if(ch_id==139) {x=-3889.09;		y=-3889.09;	z=-858;}
	if(ch_id==140) {x=3889.09;		y=-3889.09;	z=-2108;}
	if(ch_id==141) {x=-3889.09;		y=-3889.09;	z=-3358;}
	if(ch_id==142) {x=-5500;		y=dx;		z=2892;}
	if(ch_id==143) {x=-3889.09;		y=-3889.09;	z=4142;}
	if(ch_id==144) {x=3889.09;		y=-3889.09;	z=4142;}
	if(ch_id==145) {x=3889.09;		y=3889.09;	z=2892;}
	if(ch_id==146) {x=3889.09;		y=-3889.09;	z=1642;}
	if(ch_id==147) {x=3889.09;		y=3889.09;	z=392;}
	if(ch_id==148) {x=-3250;		y=dx;		z=-4608;}
	if(ch_id==149) {x=-5500;		y=dx;		z=-2108;}
	if(ch_id==150) {x=-5500;		y=dx;		z=-858;}
	if(ch_id==152) {x=5500;		y=dx;		z=1642;}
	if(ch_id==153) {x=5500;		y=dx;		z=-2108;}
	if(ch_id==154) {x=dx;			y=-4500;	z=-4608;}
	if(ch_id==155) {x=3181.98;		y=3181.98;	z=-4608;}
	if(ch_id==156) {x=-3889.09;		y=3889.09;	z=1642;}
	if(ch_id==157) {x=dx;			y=-3250;	z=-4608;}
	if(ch_id==158) {x=3889.09;		y=-3889.09;	z=2892;}
	if(ch_id==159) {x=-3181.98;		y=-3181.98;	z=-4608;}
	if(ch_id==160) {x=dx;			y=5500;		z=-858;}
	if(ch_id==161) {x=3889.09;		y=3889.09;	z=-3358;}
	if(ch_id==162) {x=-3889.09;		y=-3889.09;	z=-2108;}
	if(ch_id==163) {x=dx;			y=5500;		z=-2108;}
	if(ch_id==164) {x=-3889.09;		y=3889.09;	z=4142;}
	if(ch_id==165) {x=dx;			y=5500;		z=4142;}
	if(ch_id==166) {x=3889.09;		y=-3889.09;	z=392;}
	if(ch_id==167) {x=-5500;		y=dx;		z=-3358;}
	if(ch_id>167&&ch_id<192) {x=def;	y=def;		z=def;}
	if(ch_id==192) {x=dx;			y=-2000;	z=-4608;}
	if(ch_id==193) {x=5500;		y=dx;		z=-858;}
	if(ch_id==194) {x=-3889.09;		y=-3889.09;	z=392;}
	if(ch_id==195) {x=2298.1;		y=-2298.1;	z=-4608;}
	if(ch_id==196) {x=-3181.98;		y=3181.98;	z=-4608;}
	if(ch_id==197) {x=-5500;		y=dx;		z=392;}
	if(ch_id==198) {x=dx;			y=-5500;	z=-3358;}
	if(ch_id==199) {x=-4500;		y=dx;		z=-4608;}
	if(ch_id==200) {x=1414.21;		y=1414.21;	z=-4608;}
	if(ch_id==201) {x=3889.09;		y=3889.09;	z=4142;}
	if(ch_id==202) {x=dx;			y=-5500;	z=1642;}
	if(ch_id==203) {x=dx;			y=-5500;	z=-858;}
	if(ch_id==204) {x=3250;		y=dx;		z=-4608;}
	if(ch_id==205) {x=1414.21;		y=-1414.21;	z=-4608;}
	if(ch_id==206) {x=dx;			y=5500;		z=392;}
	if(ch_id==207) {x=-5500;		y=dx;		z=1642;}
	if(ch_id==208) {x=3889.09;		y=3889.09;	z=-2108;}
	if(ch_id==209) {x=-5500;		y=dx;		z=4142;}
	if(ch_id==210) {x=-2298.1;		y=-2298.1;	z=-4608;}
	if(ch_id==211) {x=-1414.21;		y=1414.21;	z=-4608;}
	if(ch_id==212) {x=-3889.09;		y=3889.09;	z=392;}
	if(ch_id==213) {x=4500;		y=dx;		z=-4608;}
	if(ch_id==214) {x=dx;			y=5500;		z=-3358;}
	if(ch_id==215) {x=2000;		y=dx;		z=-4608;}
	if(ch_id==216) {x=3889.09;		y=3889.09;	z=-858;}
	if(ch_id==217) {x=dx;			y=5500;		z=2892;}
	if(ch_id==218) {x=5500;		y=dx;		z=2892;}
	if(ch_id==219) {x=3889.09;		y=-3889.09;	z=-3358;}
	if(ch_id==220) {x=-3889.09;		y=3889.09;	z=2892;}
	if(ch_id==221) {x=-3889.09;		y=3889.09;	z=-858;}
	if(ch_id==222) {x=3889.09;		y=3889.09;	z=1642;}
	if(ch_id==223) {x=dx;			y=5500;		z=1642;}
	if(ch_id==224) {x=dx;			y=-5500;	z=392;}
	if(ch_id==225) {x=3181.98;		y=-3181.98;	z=-4608;}
	if(ch_id==226) {x=dx;			y=-5500;	z=-2108;}
	if(ch_id==227) {x=-3889.09;		y=3889.09;	z=-3358;}
	if(ch_id==228) {x=dx;			y=-5500;	z=2892;}
	if(ch_id==229) {x=-3889.09;		y=3889.09;	z=-2108;}
	if(ch_id==230) {x=dx;			y=-5500;	z=4142;}
	if(ch_id==231) {x=-2000;		y=dx;		z=-4608;}	       
      }
      if (detector=="tpc") {
	// XY coordinates used in XY-reconstruction.
	if(ch_id==0)    {x=0.;  y=-4.; z=-1.;}
	if(ch_id==1)    {x=-1.; y=-3.; z=-1.;}
	if(ch_id==2)    {x=-2.; y=-2.; z=-1.;}
	if(ch_id==3)    {x=-2.; y=0.;  z=-1.;}
	if(ch_id==4)    {x=-2.; y=2.;  z=-1.;}
	if(ch_id==5)    {x=-1.; y=1.;  z=-1.;}
	if(ch_id==6)    {x=-1.; y=-1.; z=-1.;}
	if(ch_id==7)    {x=0.;  y=-2.; z=-1.;}
	if(ch_id==8)    {x=1.;  y=-3.; z=-1.;}
	if(ch_id==9)    {x=2.;  y=-2.; z=-1.;}
	if(ch_id==10)   {x=1.;  y=-1.; z=-1.;}
	if(ch_id==11)   {x=0.;  y=0.;  z=-1.;}
	if(ch_id==12)   {x=0.;  y=2.;  z=-1.;}
	if(ch_id==13)   {x=-1.; y=3.;  z=-1.;}
	if(ch_id==14)   {x=0.;  y=4.;  z=-1.;}
	if(ch_id==15)   {x=1.;  y=3.;  z=-1.;}
	if(ch_id==16)   {x=1.;  y=1.;  z=-1.;}
	if(ch_id==17)   {x=2.;  y=0.;  z=-1.;}
	if(ch_id==18)   {x=2.;  y=2.;  z=-1.;}
	if(ch_id==19)   {x=-2.; y=-2.; z=1.;}
	if(ch_id==20)   {x=-1.; y=-3.; z=1.;}
	if(ch_id==21)   {x=0.;  y=-4.; z=1.;}
	if(ch_id==22)   {x=1.;  y=-3.; z=1.;}
	if(ch_id==23)   {x=2.;  y=-2.; z=1.;}
	if(ch_id==24)   {x=1.;  y=-1.; z=1.;}
	if(ch_id==25)   {x=0.;  y=-2.; z=1.;}
	if(ch_id==26)   {x=-1.; y=-1.; z=1.;}
	if(ch_id==27)   {x=-2.; y=0.;  z=1.;}
	if(ch_id==28)   {x=-2.; y=2.;  z=1.;}
	if(ch_id==29)   {x=-1.; y=1.;  z=1.;}
	if(ch_id==30)   {x=0.;  y=0.;  z=1.;}
	if(ch_id==31)   {x=1.;  y=1.;  z=1.;}
	if(ch_id==32)   {x=2.;  y=0.;  z=1.;}
	if(ch_id==33)   {x=2.;  y=2.;  z=1.;}
	if(ch_id==34)   {x=1.;  y=3.;  z=1.;}
	if(ch_id==35)   {x=0.;  y=2.;  z=1.;}
	if(ch_id==36)   {x=-1.; y=3.;  z=1.;}
	if(ch_id==37)   {x=0.;  y=4.;  z=1.;}
	x *= Dx;
	y *= Dy;
	z *= Dz;
	if(fabs(x)<dx) x=dx; // prevent divide by 0 error
	if(fabs(y)<dx) y=dx; // prevent divide by 0 error
	double x0 = x;
	double y0 = y;
	double r0 = GetPMTrho();
	x = RotateByOffsetX(x0,y0,r0);
	y = RotateByOffsetY(x0,y0,r0);
	//	  std::cout<<"New xyr: "<<x<<" "<<y<<" "<<GetPMTrho()<<"\n";
      }
	
    }
      
  private:
    std::string detector;
    int    ch_id;  // channel id
    double pi;     // 3.14
    double def;    // default coordinate value to return on error
    double dx;     // small coordinate value to avoid divide-by-0 errorrs
    double Dx,Dy;  // spacing between tpc pmts in the x and y direction
    double Dz;     // height of tpc/2
    double x;      // pmt x position
    double y;      // pmt y position
    double z;      // pmt z position

    double tpc_lsv_offset; // azimuthal offset between tpc and lsv in rad
    double OffsetAngle(double x0, double y0) {
      //	std::cout<<"\nOrig xyr: "<<x0<<" "<<y0<<" "<<GetPMTrho()<<"\n";
      //	std::cout<<"Orig ang: "<<atan(y0/x0)*360./6.28<<"\n";
      //	std::cout<<"New ang: "<<(atan(y0/x0)+tpc_lsv_offset)*360./6.28<<"\n";	
      double ang = atan(y0/x0);
      if (x0<0) ang += pi;
      return ang + tpc_lsv_offset;
    }
    double RotateByOffsetX(double x0, double y0, double r0) {
      return r0*cos(OffsetAngle(x0,y0));
    }
    double RotateByOffsetY(double x0, double y0, double r0) {
      return r0*sin(OffsetAngle(x0,y0));
    }
      
  };// End of PMT_positions class    
}// End of display namespace

#endif
