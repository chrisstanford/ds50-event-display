#ifndef ChannelColors_hh
#define ChannelColors_hh

#include <string>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TList.h"

namespace display{
  inline std::string GetChannelColor(std::string detector, int ch_id) {
    if(detector=="lsv"||detector=="wt"||detector=="od") {
      if(ch_id==0) return "#ff4040";
      if(ch_id==1) return "#731d1d";
      if(ch_id==2) return "#e57373";
      if(ch_id==3) return "#733939";
      if(ch_id==4) return "#cc1b00";
      if(ch_id==5) return "#bf4330";
      if(ch_id==6) return "#b26559";
      if(ch_id==7) return "#ff4400";
      if(ch_id==8) return "#992900";
      if(ch_id==9) return "#d96236";
      if(ch_id==10) return "#73341d";
      if(ch_id==11) return "#d9896c";
      if(ch_id==12) return "#7f5140";
      if(ch_id==13) return "#cc5200";
      if(ch_id==14) return "#732e00";
      if(ch_id==15) return "#f2853d";
      if(ch_id==16) return "#a65b29";
      if(ch_id==17) return "#d9986c";
      if(ch_id==18) return "#996b4d";
      if(ch_id==19) return "#664733";
      if(ch_id==20) return "#ff8800";
      if(ch_id==21) return "#bf6600";
      if(ch_id==22) return "#a66c29";
      if(ch_id==23) return "#000000";
      if(ch_id==24) return "#734b1d";
      if(ch_id==25) return "#d9a66c";
      if(ch_id==26) return "#cc8800";
      if(ch_id==27) return "#a67c29";
      if(ch_id==28) return "#736039";
      if(ch_id==29) return "#cca300";
      if(ch_id==30) return "#d9c36c";
      if(ch_id==31) return "#ccbe00";
      if(ch_id==32) return "#998f00";
      if(ch_id==33) return "#736d1d";
      if(ch_id==34) return "#e6f23d";
      if(ch_id==35) return "#d2d96c";
      if(ch_id==36) return "#94994d";
      if(ch_id==37) return "#526600";
      if(ch_id==38) return "#77b300";
      if(ch_id==39) return "#b6f23d";
      if(ch_id==40) return "#607339";
      if(ch_id==41) return "#89b359";
      if(ch_id==42) return "#388c00";
      if(ch_id==43) return "#aaf279";
      if(ch_id==44) return "#62d936";
      if(ch_id==45) return "#34731d";
      if(ch_id==46) return "#00ff00";
      if(ch_id==47) return "#59b371";
      if(ch_id==48) return "#00a642";
      if(ch_id==49) return "#3df285";
      if(ch_id==50) return "#00733d";
      if(ch_id==51) return "#397358";
      if(ch_id==52) return "#36d9a3";
      if(ch_id==53) return "#29a68d";
      if(ch_id==54) return "#73e6cf";
      if(ch_id==55) return "#00ffee";
      if(ch_id==56) return "#000000";
      if(ch_id==57) return "#000000";
      if(ch_id==58) return "#000000";
      if(ch_id==59) return "#000000";
      if(ch_id==60) return "#000000";
      if(ch_id==61) return "#000000";
      if(ch_id==62) return "#000000";
      if(ch_id==63) return "#000000";
      if(ch_id==64) return "#00736b";
      if(ch_id==65) return "#00eeff";
      if(ch_id==66) return "#008f99";
      if(ch_id==67) return "#6cd2d9";
      if(ch_id==68) return "#336366";
      if(ch_id==69) return "#00ccff";
      if(ch_id==70) return "#0085a6";
      if(ch_id==71) return "#005c73";
      if(ch_id==72) return "#00aaff";
      if(ch_id==73) return "#004d73";
      if(ch_id==74) return "#36a3d9";
      if(ch_id==75) return "#0088ff";
      if(ch_id==76) return "#0058a6";
      if(ch_id==77) return "#003d73";
      if(ch_id==78) return "#000000";
      if(ch_id==79) return "#307cbf";
      if(ch_id==80) return "#466c8c";
      if(ch_id==81) return "#0066ff";
      if(ch_id==82) return "#002e73";
      if(ch_id==83) return "#7999f2";
      if(ch_id==84) return "#000f73";
      if(ch_id==85) return "#3043bf";
      if(ch_id==86) return "#0000ff";
      if(ch_id==87) return "#0000d9";
      if(ch_id==88) return "#0000bf";
      if(ch_id==89) return "#23238c";
      if(ch_id==90) return "#6c6cd9";
      if(ch_id==91) return "#6236d9";
      if(ch_id==92) return "#493973";
      if(ch_id==93) return "#7453a6";
      if(ch_id==94) return "#ba79f2";
      if(ch_id==95) return "#aa00ff";
      if(ch_id==96) return "#6f00a6";
      if(ch_id==97) return "#4d0073";
      if(ch_id==98) return "#a336d9";
      if(ch_id==99) return "#673973";
      if(ch_id==100) return "#ee00ff";
      if(ch_id==101) return "#9d29a6";
      if(ch_id==102) return "#ea79f2";
      if(ch_id==103) return "#a653a0";
      if(ch_id==104) return "#73005c";
      if(ch_id==105) return "#d936b8";
      if(ch_id==106) return "#990066";
      if(ch_id==107) return "#f279ca";
      if(ch_id==108) return "#733960";
      if(ch_id==109) return "#73003d";
      if(ch_id==110) return "#d9368d";
      if(ch_id==111) return "#b35989";
      if(ch_id==112) return "#d90057";
      if(ch_id==113) return "#a6295b";
      if(ch_id==114) return "#f279aa";
      if(ch_id==115) return "#d9003a";
      if(ch_id==116) return "#b35971";
      if(ch_id==117) return "#733949";
      if(ch_id==118) return "#990014";
      if(ch_id==119) return "#a6293a";
      if(ch_id==120) return "#000000";
      if(ch_id==121) return "#000000";
      if(ch_id==122) return "#000000";
      if(ch_id==123) return "#000000";
      if(ch_id==124) return "#000000";
      if(ch_id==125) return "#000000";
      if(ch_id==126) return "#000000";
      if(ch_id==127) return "#000000";
      if(ch_id==128) return "#ff0000";
      if(ch_id==129) return "#d90000";
      if(ch_id==130) return "#7f0000";
      if(ch_id==131) return "#7f4040";
      if(ch_id==132) return "#bf4330";
      if(ch_id==133) return "#f28979";
      if(ch_id==134) return "#ff7340";
      if(ch_id==135) return "#a66953";
      if(ch_id==136) return "#664133";
      if(ch_id==137) return "#bf4d00";
      if(ch_id==138) return "#7f3300";
      if(ch_id==139) return "#f2aa79";
      if(ch_id==140) return "#d97400";
      if(ch_id==141) return "#663600";
      if(ch_id==142) return "#ffa640";
      if(ch_id==143) return "#996326";
      if(ch_id==144) return "#d9a66c";
      if(ch_id==145) return "#bf8f30";
      if(ch_id==146) return "#664d1a";
      if(ch_id==147) return "#ffd580";
      if(ch_id==148) return "#99804d";
      if(ch_id==149) return "#ffcc00";
      if(ch_id==150) return "#ffee00";
      if(ch_id==151) return "#bfb300";
      if(ch_id==152) return "#7f7700";
      if(ch_id==153) return "#fff780";
      if(ch_id==154) return "#666333";
      if(ch_id==155) return "#adb359";
      if(ch_id==156) return "#cef23d";
      if(ch_id==157) return "#5d8c00";
      if(ch_id==158) return "#758c46";
      if(ch_id==159) return "#a6d96c";
      if(ch_id==160) return "#3ad900";
      if(ch_id==161) return "#20802d";
      if(ch_id==162) return "#7fff91";
      if(ch_id==163) return "#336641";
      if(ch_id==164) return "#00b347";
      if(ch_id==165) return "#73e6a1";
      if(ch_id==166) return "#008c5e";
      if(ch_id==167) return "#60bf9f";
      if(ch_id==168) return "#000000";
      if(ch_id==169) return "#000000";
      if(ch_id==170) return "#000000";
      if(ch_id==171) return "#000000";
      if(ch_id==172) return "#000000";
      if(ch_id==173) return "#000000";
      if(ch_id==174) return "#000000";
      if(ch_id==175) return "#000000";
      if(ch_id==176) return "#000000";
      if(ch_id==177) return "#000000";
      if(ch_id==178) return "#000000";
      if(ch_id==179) return "#000000";
      if(ch_id==180) return "#000000";
      if(ch_id==181) return "#000000";
      if(ch_id==182) return "#000000";
      if(ch_id==183) return "#000000";
      if(ch_id==184) return "#000000";
      if(ch_id==185) return "#000000";
      if(ch_id==186) return "#000000";
      if(ch_id==187) return "#000000";
      if(ch_id==188) return "#000000";
      if(ch_id==189) return "#000000";
      if(ch_id==190) return "#000000";
      if(ch_id==191) return "#000000";
      if(ch_id==192) return "#00ffcc";
      if(ch_id==193) return "#20806c";
      if(ch_id==194) return "#00f2e2";
      if(ch_id==195) return "#00cad9";
      if(ch_id==196) return "#207980";
      if(ch_id==197) return "#00add9";
      if(ch_id==198) return "#0091d9";
      if(ch_id==199) return "#206080";
      if(ch_id==200) return "#0074d9";
      if(ch_id==201) return "#80c4ff";
      if(ch_id==202) return "#46628c";
      if(ch_id==203) return "#1a2e66";
      if(ch_id==204) return "#80a2ff";
      if(ch_id==205) return "#001dd9";
      if(ch_id==206) return "#3d55f2";
      if(ch_id==207) return "#263699";
      if(ch_id==208) return "#4d5799";
      if(ch_id==209) return "#7373e6";
      if(ch_id==210) return "#13008c";
      if(ch_id==211) return "#381a66";
      if(ch_id==212) return "#473366";
      if(ch_id==213) return "#9d3df2";
      if(ch_id==214) return "#b073e6";
      if(ch_id==215) return "#5e008c";
      if(ch_id==216) return "#7e468c";
      if(ch_id==217) return "#ee00ff";
      if(ch_id==218) return "#bf00b3";
      if(ch_id==219) return "#66005f";
      if(ch_id==220) return "#e673de";
      if(ch_id==221) return "#8c005e";
      if(ch_id==222) return "#f23db6";
      if(ch_id==223) return "#e5007a";
      if(ch_id==224) return "#8c466c";
      if(ch_id==225) return "#ff0066";
      if(ch_id==226) return "#e673a1";
      if(ch_id==227) return "#f20041";
      if(ch_id==228) return "#8c0025";
      if(ch_id==229) return "#bf3056";
      if(ch_id==230) return "#661a2e";
      if(ch_id==231) return "#e57382";
    }
    if(detector=="tpc") {
      if(ch_id==0)  return "#ff0000";
      if(ch_id==1)  return "#f28979";
      if(ch_id==2)  return "#992900";
      if(ch_id==3)  return "#ff6600";
      if(ch_id==4)  return "#733f1d";
      if(ch_id==5)  return "#f2aa79";
      if(ch_id==6)  return "#b25f00";
      if(ch_id==7)  return "#ffaa00";
      if(ch_id==8)  return "#664400";
      if(ch_id==9)  return "#a69553";
      if(ch_id==10) return "#ffee00";
      if(ch_id==11) return "#8f9900";
      if(ch_id==12) return "#baf279";
      if(ch_id==13) return "#369926";
      if(ch_id==14) return "#3df23d";
      if(ch_id==15) return "#33663a";
      if(ch_id==16) return "#3df29d";
      if(ch_id==17) return "#00a685";
      if(ch_id==18) return "#00ffee";
      if(ch_id==19) return "#00ccff";
      if(ch_id==20) return "#396773";
      if(ch_id==21) return "#00aaff";
      if(ch_id==22) return "#004466";
      if(ch_id==23) return "#0066ff";
      if(ch_id==24) return "#1d3473";
      if(ch_id==25) return "#4d6199";
      if(ch_id==26) return "#0000ff";
      if(ch_id==27) return "#0000e6";
      if(ch_id==28) return "#000066";
      if(ch_id==29) return "#660099";
      if(ch_id==30) return "#ac60bf";
      if(ch_id==31) return "#ee00ff";
      if(ch_id==32) return "#660052";
      if(ch_id==33) return "#ff00aa";
      if(ch_id==34) return "#ff0044";
      if(ch_id==35) return "#bf3056";
      if(ch_id==36) return "#8c4659";
      if(ch_id==37) return "#73000f";
    }
    return "#000000";
  }
  inline void SetChannelColors(std::string detector,TMultiGraph* mg) {
    //      std::cout<<"Initializing colors";
    TList* mg_list = mg->GetListOfGraphs();
    for(int i=0;i<mg_list->GetSize();i++) { 
      if (!mg_list->At(i)) continue;
      //	std::cout<<"Coloring: "<<mg_list->At(i)->GetName()<<std::endl;
      TGraph* tg = (TGraph*)(mg_list->At(i));
      tg->SetLineColor(TColor::GetColor
		       (display::GetChannelColor
			(detector,i).c_str()));
    }
  } 
  /*
    inline void InitializeColors() {
    for(int i=0;i<38;i++) {
    TColor::GetColor(display::GetChannelColor("tpc",i).c_str());
    }
    for(int i=0;i<232;i++) {
    TColor::GetColor(display::GetChannelColor("od",i).c_str());
    }      
    }
  */
}
#endif
