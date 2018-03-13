#ifndef _QC_H_
#include "qc.h"
#endif

#include "version.h"

char *BASE[5]={"A","C","G","T","N"};
char *BASECOLOR[5]={"#CC0000","#CC6600","#000066","#009900","#666666"};
char *QUALCOLOR[5]={"#CC9966","#CC6666","#000066","#CC6666","#66CCCC"};
/*输出html头*/
void html_head()
{
char *name = title ? title : "FastProNGS";
char head[] =
"<html>"
"<head>"
"  <meta http-equiv='content-type' content='text/html;charset=utf-8' />"
"  <title>FastProNGS</title>"
"  <script src='https://cdn.plot.ly/plotly-latest.min.js'></script>"
"  <style>"
"    body {"
"      margin: 0 auto;"
"      text-align: center;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    #container {"
"      text-align: left;"
"      margin: auto;"
"      width: 1300px;"
"    }"
""
"    table {"
"      border: 1px solid #999999;"
"      padding: 2x;"
"      border-collapse: collapse;"
"      margin: 9px;"
"    }"
""
"    th {"
"      text-align: left;"
"      border: 1px solid #dddddd;"
"      padding: 5px;"
"      font-size: 12px;"
"      font-weight: bold;"
"    }"
""
"    td {"
"      border: 1px solid #dddddd;"
"      padding: 5px 20px;"
"      font-size: 12px;"
"    }"
""
"    .tdt {"
"      text-align: left;"
"      border: 1px solid #dddddd;"
"      padding: 5px 20px;"
"      font-size: 13px;"
"      font-weight: bold;"
"    }"
""
"    #head {"
"      padding: 8px;"
"      margin: 0;"
"      font-size: 24px;"
"      background-color: #003366;"
"      color: #CCCCCC;"
"    }"
""
"    #summary {"
"      margin: 0;"
"      margin-top: 4px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    #adapter {"
"      margin: 0;"
"      margin-top: 4px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    .adapter-table {"
"      border-collapse: collapse;"
"      padding: 0px;"
"      border: none;"
"    }"
""
"    .adapter-table td {"
"      font-size: 13px;"
"      line-height: 5px;"
"      border: none;"
"    }"
""
"    .adapter-table span {"
"      margin: 0px;"
"      padding: 0px;"
"    }"
""
"    .chrA {"
"      color: #990033;"
"    }"
""
"    .chrC {"
"      color: #339966;"
"    }"
""
"    .chrG {"
"      color: #3300CC;"
"      margin: 0px;"
"      padding: 0px;"
"    }"
""
"    .chrT {"
"      color: #FF9900;"
"    }"
""
"    .chrN {"
"      color: #666666;"
"    }"
""
"    .chr_ {"
"      color: #999999;"
"    }"
""
"    .title {"
"      font-weight: bold;"
"      padding: 8px;"
"      border-bottom: #CCCCCC solid 1px;"
"    }"
""
"    .subtitle {"
"      padding: 8px;"
"      width: 100%%;"
"      clear:both; "
"    }"
"    #stat {"
"      margin: 0;"
"      margin-top: 4px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    #base {"
"      margin: 0;"
"      margin-top: 4px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    #quality {"
"      margin: 0;"
"      margin-top: 4px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    #foot {"
"      margin: 0;"
"      margin-top: 10px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
""
"    .after {"
"      color: #990033;"
"    }"
""
"    #piebase{"
"      margin: 8px;"
"      border: 1px solid #CCCCCC;"
"    }"
""
"    .adapter{"
"      width:100%%;"
"      border-top: 1px dashed #CCCCCC;"
"      border-bottom: 1px dashed #CCCCCC;"
"    }"
""
"    .bqpic{"
"      width:97%%;"
"      float:left;"
"    }"
""
"    .bqpic2{"
"      width:48%%;"
"      float:left;"
"    }"
""
"    .bqpic3{"
"      width:24%%;"
"      float:left;"
"    }"
"    #cmd{"
"      margin: 0;"
"      margin-top: 4px;"
"      margin-bottom: 9px;"
"      border: 1px solid #CCCCCC;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"    }"
"    #cmd span{"
"      margin: 10px;"
"      padding-top:10px;"
"      padding-bottom:10px;"
"      font-family: Arail, 'Liberation Mono', Menlo, Courier, monospace;"
"      font-size:12px;"
"      font-weight:bold;"
"      color: #555555;"
"    }"
""
"  </style>"
"</head>"
"<body>"
""
"  <div id='container'>"
"    <div id='head'>FastProNGS report</div>";
fprintf(HTML, head, name);
} /*html_head */

/*输出概要*/
void html_summary()
{
int i=0,j=0;
char fmsga[100] = {0};
char fmsgb[100] = {0};
fprintf(HTML,
"    <div id='summary'>"
"      <div class='title'>Summary(before/<span class='after'>after</span>)</div>");
/*输出read 1和2的汇总信息*/
conv_units(summary[0][0].reads, fmsga);
conv_units(summary[0][1].reads, fmsgb);
fprintf(HTML,
"      <table>"
"        <tr>"
"          <td class='tdt'>Total Sequences</td>"
"          <td>%lu %s(%s)/<span class='after'>%lu %s(%s)</span></td>",
summary[0][0].reads,paired?"paired":"",fmsga,
summary[0][1].reads,paired?"paired":"",fmsgb);

conv_units(summary[0][0].bases+summary[1][0].bases, fmsga);
conv_units(summary[0][1].bases+summary[1][1].bases, fmsgb);
fprintf(HTML,
"        <tr>"
"          <td class='tdt'>Total Base</td>"
"          <td>%lu bp(%s)/<span class='after'>%lu bp(%s)</span></td>",
summary[0][0].bases+summary[1][0].bases,fmsga,summary[0][1].bases+summary[1][1].bases,fmsgb);
fprintf(HTML,
"        </tr>"
"        <tr>"
"          <td class='tdt'>Sequence length</td>"
"          <td>%s %lu</td>",
paired?"PE":"SE",summary[0][0].maxlen);
fprintf(HTML,
"        </tr>"
"        <tr>"
"          <td class='tdt'>Total %%GC</td>"
"          <td>%.2f/<span class='after'>%.2f</span></td>",
((float)(summary[0][0].GC+summary[1][0].GC)/(summary[0][0].bases+summary[1][0].bases))*100,((float)(summary[0][1].GC+summary[1][1].GC)/(summary[0][1].bases+summary[1][1].bases))*100);
fprintf(HTML,
"        </tr>"
"        <tr>"
"          <td class='tdt'>Total %%Q20</td>"
"          <td>%.2f/<span class='after'>%.2f</span></td>",
((float)(summary[0][0].q20+summary[1][0].q20)/(summary[0][0].bases+summary[1][0].bases))*100,((float)(summary[0][1].q20+summary[1][1].q20)/(summary[0][1].bases+summary[1][1].bases))*100);
fprintf(HTML,
"        </tr>"
"        <tr>"
"          <td class='tdt'>Total %%Q30</td>"
"          <td>%.2f/<span class='after'>%.2f</span></td>",
((float)(summary[0][0].q30+summary[1][0].q30)/(summary[0][0].bases+summary[1][0].bases))*100,((float)(summary[0][1].q30+summary[1][1].q30)/(summary[0][1].bases+summary[1][1].bases))*100);
fprintf(HTML,
"        </tr>"
"        <tr>"
"          <td class='tdt'>Total %%N</td>"
"          <td>%.2f/<span class='after'>%.2f</span></td>",
((float)(summary[0][0].N+summary[1][0].N)/(summary[0][0].bases+summary[1][0].bases))*100,((float)(summary[0][1].N+summary[1][1].N)/(summary[0][1].bases+summary[1][1].bases))*100);
fprintf(HTML,
"        </tr>"
"      </table>");
fprintf(HTML,
"      <table>"
"        <tr>"
"          <td class='tdt'>Before/<span class='after'>after</span></td>"
"          <td class='tdt'>Sequences</td>"
"          <td class='tdt'>Bases</td>"
"          <td class='tdt'>%%Q20</td>"
"          <td class='tdt'>%%Q30</td>"
"          <td class='tdt'>%%GC</td>"
"          <td class='tdt'>%%N</td>"
"        </tr>"
);
/*输出各个read的信息*/
for(i=0;i<=paired;i++){
conv_units(summary[0][0].reads, fmsga);
conv_units(summary[0][1].reads, fmsgb);
fprintf(HTML,
"        <tr>"
"          <td class='tdt'>read %d</td>"
"          <td class='tdt'>%lu(%s)/<span class='after'>%lu(%s)</span></td>",
i+1,summary[i][0].reads,fmsga,summary[i][1].reads, fmsgb);
conv_units(summary[i][0].bases, fmsga);
conv_units(summary[i][1].bases, fmsgb);
fprintf(HTML,
"          <td class='tdt'>%lu(%s)/<span class='after'>%lu(%s)</span></td>",
summary[i][0].bases, fmsga,summary[i][1].bases, fmsgb);
fprintf(HTML,
"          <td class='tdt'>%.2f/<span class='after'>%.2f</span></td>",
((float)summary[i][0].q20/summary[i][0].bases)*100,((float)summary[i][1].q20/summary[i][1].bases)*100);
fprintf(HTML,
"          <td class='tdt'>%.2f/<span class='after'>%.2f</span></td>",
((float)summary[i][0].q30/summary[i][0].bases)*100,((float)summary[i][1].q30/summary[i][1].bases)*100);
fprintf(HTML,
"          <td class='tdt'>%.2f/<span class='after'>%.2f</span></td>",
((float)summary[i][0].GC/summary[i][0].bases)*100,((float)summary[i][1].GC/summary[i][1].bases)*100);
fprintf(HTML,
"          <td class='tdt'>%.2f/<span class='after'>%.2f</span></td>"
"        </tr>",
((float)summary[i][0].N/summary[i][0].bases)*100,((float)summary[i][1].N/summary[i][1].bases)*100);
}/*i*/
fprintf(HTML,
"      </table>");

/*画ACGTN饼图*/
fprintf(HTML,
"      <div id='piebase'>"
"        <div id='pie_base_pic'></div>"
"        <script type='text/javascript'>"
"          var pie_base_pic_data=[");
for(i=0;i<=paired;i++){
for(j=0;j<2;j++){
fprintf(HTML,
"%s{values:[%lu,%lu,%lu,%lu,%lu],"
"labels:['A','C','G','T','N'],"
"domain:{x:[%f,%f]},"
"name:'Before',"
"marker: {"
"  colors:['#00CCCC','#99CCFF','#FF9966','#CC0066','#999999']"
"},"
"hoverinfo:'label+percent+name',"
// "hole:.4,"
"type:'pie'}",
i||j?",":"",
summary[i][j].A,
summary[i][j].C,
summary[i][j].G,
summary[i][j].T,
summary[i][j].N,i*0.5+j*0.25,i*0.5+(j+1)*0.25);
}/*j*/
}/*i*/
fprintf(HTML,
"];"
"var layout={"
"   title:'Base Percentage',"
"   titlefont: {"
"     family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
"     size: 14,"
"     color: '#330033'"
"  },"
"  legend: {"
"    x: -0.05,"
"    y: 1.1,"
"  },"
"  'annotations':[");
for(i=0;i<=paired;i++){
for(j=0;j<2;j++){
fprintf(HTML,
"  %s{"
"    font:{family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\",size:11},"
"    showarrow:false,"
"    text:'Read %d(%s)',"
"    x:%f,"
"    y:-0.2"
"  }",i||j?",":"",i+1,j?"after":"before",i*0.5+j*0.25+0.08);

}/*j*/
}/*i*/
fprintf(HTML,
"  ],"
//"  paper_bgcolor: '#EEEEEE',"
"  height: 310"
//"  width: 1200"
"};"
"Plotly.newPlot('pie_base_pic', pie_base_pic_data, layout);");

fprintf(HTML,
"        </script>"
"      </div>");/*画ACGTN饼图*/

fprintf(HTML,
"    </div>");/*summary*/
} /*for html_summary*/

/*带接头和过滤掉read统计*/
void html_rstat()  /*统计*/
{
int i = 0, j = 0;
uli total=0ul,clean=0ul;
for(i=0;i<TF;i++){
  total+=t_dstat[i];
}/*i*/
clean=summary[0][0].reads-total;
if(total){
  fprintf(HTML,
  "<div id='stat'>"
  "<div class='title'>Statistics</div>"
  "<div id='pie_stat_pic' class='bqpic'></div>"
  "<script type='text/javascript'>"
  "var pie_stat_dat=[{values:[");
  for(i=0;i<TF;i++){
    if(t_dstat[i])fprintf(HTML,"%lu,",t_dstat[i]);
  }/*i*/
  fprintf(HTML,"%lu",clean);
  fprintf(HTML,"],labels:[");
  for(i=1;i<TF;i++){
    if(t_dstat[i]){
      fprintf(HTML,"'");
      if(i&AN)fprintf(HTML,"Adapters + ");
      if(i&UNQ)fprintf(HTML,"%d,%g + ",quality_threshold,quality_ratio);
      if(i&NR)fprintf(HTML,"N%% + ");
      if(i&GCC)fprintf(HTML,"GC%% + ");
      if(i&MINL)fprintf(HTML,"Min length + ");
      if(i&MAXL)fprintf(HTML,"Max length + ");
      fprintf(HTML,"',");
    }
  }/*i*/
  fprintf(HTML,"'Clean Read'],"
  "name:'Stat',"
  "marker: {"
  "  colors:['#99CC99','#CC6666','#9966CC','#FF9999','#FFCC99','#CCCCCC','#99FFCC']"
  "},"
  "hoverinfo:'label+percent+name',"
  "type:'pie'}];");  
  fprintf(HTML,
  "var pie_stat_layout={"
  "title:'Statistics Percentage',"
  "margin: {"
  "  b: 3"
  "},"
  "titlefont: {"
  "  family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
  "  size: 14,"
  "  color: '#330033'"
  "},"
  "legend: {"
  "},"
  "height:350"
  "};"
  "Plotly.newPlot('pie_stat_pic', pie_stat_dat, pie_stat_layout);"
  "</script>");
  fprintf(HTML,
  //"<div class='bqpic2'>"
  //"<table><tr><td colspan='2' align='center'>xxxx</td><td>xxxx</td></tr><tr><td>xxxx</td><td>xxxx</td></tr><tr><td>xxxx</td><td>xxxx</td></tr><tr><td>xxxx</td><td>xxxx</td></tr></table>"
  //"</div>"
  "<div class='subtitle'></div>");
  fprintf(HTML,
  "</div>");
}/*if total*/
}/*html_rstat()*/
/* 打印接头信息 */
void html_adapter()
{
int i = 0, j = 0, k = 0, len = 0, b = 0,max=0;
uli f=0ul,total=0ul,temp=0ul;
fprintf(HTML,
"    <div id='adapter'>"
"      <div class='title'>Adapter</div>");
for (i = 0; i <= paired; i++){
  if (fronts[i] != NULL)
  { /*5' 接头*/
    fprintf(HTML,
    "      <div class='subtitle'>Read %d</div>",i+1);
    for (j = 0; j < frontn[i]; j++)
    {
      len = strlen(fronts[i][j]);
      fprintf(HTML,
      "      <div class='subtitle'> 5' adapter of type '");
      psubstr2(HTML, fronts[i][j], 0, len, len);
      fprintf(HTML,
      "'</div>");
      fprintf(HTML,
      "      <div class='adapter' id='pic_read%d_type%d_5end'></div>"
      "        <script type='text/javascript'>"
      "           var read%d_type%d_5end_x = [],read%d_type%d_5end_y = [];",i,j,i,j,i,j);
      total=max=0;
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = end5Type[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)
          {
            fprintf(HTML, "read%d_type%d_5end_y.unshift(\"",i,j);
            psubstr(HTML, fronts[i][j], k, b, len);
            fprintf(HTML, "%10lu", end5TypeN[i][j][k][b]);
            fprintf(HTML,"\");",i,j);
            total+=end5TypeN[i][j][k][b];
            fprintf(HTML,"read%d_type%d_5end_x.unshift(%lu);",i,j,end5TypeN[i][j][k][b]);
            max++;
          }
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
      fprintf(HTML,
      "var read%d_type%d_5end_data = [{"
      "  x:read%d_type%d_5end_x,"
      "  y:read%d_type%d_5end_y,"
      "  type:'scatter',"
      "  marker: {"  
      "    color:  ['#0033CC','#00CCCC','#33CC66','#CC0000']"
      "  },"
      "  line:{"
      "    width:1,"
      "    color:'#666666'"
      "  }"
      "}];",i,j,i,j,i,j);

      fprintf(HTML,
      "var read%d_type%d_5end_layout = {"
      "  yaxis: {"
      "  dtick: 1,"
      "  tickfont: {"
      "    family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
      "    size: 13,"
      "    color: 'black'"
      "  }"
      "  },"
      "  margin: {"
      "    r: 10,"
      "    t: 25,"
      "    b: 40,"
      "    l: 400"
      "  },"
      "  height:%d,"
      "  title:'Total:%lu',"
      "  titlefont: {"
      "    family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
      "    size: 14,"
      "    color: '#330033'"
      "  }"
      "};"
      "Plotly.newPlot('pic_read%d_type%d_5end',read%d_type%d_5end_data, read%d_type%d_5end_layout);",i,j,100+max*12,total,i,j,i,j,i,j);
      fprintf(HTML,
      "        </script>");
    } /*for for j*/
  }   /*for if fronts*/
  if (adapts[i] != NULL)
  { /*3' 接头*/
    fprintf(HTML,
    "      <div class='subtitle'>Read %d</div>",i+1);
    for (j = 0; j < adaptn[i]; j++)
    {
      len = strlen(adapts[i][j]);
      fprintf(HTML,
      "      <div class='subtitle'> 3' adapter of type '");
      psubstr2(HTML, adapts[i][j], 0, len, len);
      fprintf(HTML,
      "'</div>");
      fprintf(HTML,
      "      <div class='adapter' id='pic_read%d_type%d_3end'></div>"
      "        <script type='text/javascript'>"
      "           var read%d_type%d_3end_x = [],read%d_type%d_3end_y = [];",i,j,i,j,i,j);
      total=max=0;
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = end3Type[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)
          {
            fprintf(HTML, "read%d_type%d_3end_y.unshift(\"",i,j);
            psubstr(HTML, adapts[i][j], k, b, len);
            fprintf(HTML, "%10lu", end3TypeN[i][j][k][b]);
            fprintf(HTML,"\");",i,j);
            total+=end3TypeN[i][j][k][b];
            fprintf(HTML,"read%d_type%d_3end_x.unshift(%lu);",i,j,end3TypeN[i][j][k][b]);
            max++;
          }
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
      fprintf(HTML,
      "var read%d_type%d_3end_data = [{"
      "  x:read%d_type%d_3end_x,"
      "  y:read%d_type%d_3end_y,"
      "  type:'scatter',"
      "  marker: {"  
      "    color:  ['#0033CC','#00CCCC','#33CC66','#CC0000']"
      "  },"
      "  line:{"
      "    width:1,"
      "    color:'#666666'"
      "  }"
      "}];",i,j,i,j,i,j);

      fprintf(HTML,
      "var read%d_type%d_3end_layout = {"
      "  yaxis: {"
      "  dtick: 1,"
      "  tickfont: {"
      "    family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
      "    size: 13,"
      "    color: 'black'"
      "  }"
      "  },"
      "  margin: {"
      "    r: 10,"
      "    t: 25,"
      "    b: 40,"
      "    l: 400"
      "  },"
      "  height:%d,"
      "  title:'Total:%lu',"
      "  titlefont: {"
      "    family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
      "    size: 14,"
      "    color: '#330033'"
      "  }"
      "};"
      "Plotly.newPlot('pic_read%d_type%d_3end',read%d_type%d_3end_data, read%d_type%d_3end_layout);",i,j,100+max*12,total,i,j,i,j,i,j);
      fprintf(HTML,
      "        </script>");
    } /*for for j*/
  }   /*for if adapts*/
  if (anywheres[i] != NULL)
  { /*5' or 3' 接头*/
    fprintf(HTML,
    "      <div class='subtitle'>Read %d</div>",i+1);
    for (j = 0; j < anywheren[i]; j++)
    {
      len = strlen(anywheres[i][j]);
      fprintf(HTML,
      "      <div class='subtitle'>5' or 3' adapter of type '");
      psubstr2(HTML, anywheres[i][j], 0, len, len);
      fprintf(HTML,
      "'</div>");
      fprintf(HTML,
      "      <div class='adapter' id='pic_read%d_type%d_53end'></div>"
      "        <script type='text/javascript'>"
      "           var read%d_type%d_53end_x = [],read%d_type%d_53end_y = [];",i,j,i,j,i,j);
      total=max=0;
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = endAnyType[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)
          {
            fprintf(HTML, "read%d_type%d_53end_y.unshift(\"",i,j);
            psubstr(HTML, anywheres[i][j], k, b, len);
            fprintf(HTML, "%10lu", endAnyTypeN[i][j][k][b]);
            fprintf(HTML,"\");",i,j);
            total+=endAnyTypeN[i][j][k][b];
            fprintf(HTML,"read%d_type%d_53end_x.unshift(%lu);",i,j,endAnyTypeN[i][j][k][b]);
            max++;
          }
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
      fprintf(HTML,
      "var read%d_type%d_53end_data = [{"
      "  x:read%d_type%d_53end_x,"
      "  y:read%d_type%d_53end_y,"
      "  type:'scatter',"
      "  marker: {"  
      "    color:  ['#0033CC','#00CCCC','#33CC66','#CC0000']"
      "  },"
      "  line:{"
      "    width:1,"
      "    color:'#666666'"
      "  }"
      "}];",i,j,i,j,i,j);

      fprintf(HTML,
      "var read%d_type%d_53end_layout = {"
      "  yaxis: {"
      "  dtick: 1,"
      "  tickfont: {"
      "    family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
      "    size: 13,"
      "    color: 'black'"
      "  },"
      "  gridcolor: '#CCCCCC',"
      "  gridwidth: 1"
      "  },"
      "  xaxis: {"
      "  gridcolor: '#CCCCCC',"
      "  gridwidth: 1"
      "  },"
      "  margin: {"
      "    r: 10,"
      "    t: 25,"
      "    b: 40,"
      "    l: 400"
      "  },"
      "  height:%d,"
      "  title:'Total:%lu',"
      "  titlefont: {"
      "    family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
      "    size: 14,"
      "    color: '#330033'"
      "  }"
      "};"
      "Plotly.newPlot('pic_read%d_type%d_53end',read%d_type%d_53end_data, read%d_type%d_53end_layout);",i,j,100+max*12,total,i,j,i,j,i,j);
      fprintf(HTML,
      "        </script>");
    } /*for for j*/
  }   /*for if fronts*/
}/*i*/

fprintf(HTML,
"    </div>");
} /*html_adapter*/
/*打印字符串一部分到某端*/
void psubstr(FILE *F, char *c, int s, int t, int l)
{
  int i = 0;
  for (i = 0; i < s; i++)
    fprintf(F, "-");

  for (i = s; i <= t; i++)
    fprintf(F, "%c",c[i]);
  
  for (i = t + 1; i < l; i++)
    fprintf(F, "-");
  
} /*psubstr*/
/*打印字符串一部分到某端*/
void psubstr2(FILE *F, char *c, int s, int t, int l)
{
  int i = 0;
  for (i = 0; i < s; i++)
    fprintf(F, "<span class='chr_'>-</span>");
  
  for (i = s; i <= t; i++)
  {
    switch(c[i]){
      case 'A':fprintf(F, "<span class='chrA'>A</span>");break;
      case 'C':fprintf(F, "<span class='chrC'>C</span>");break;
      case 'G':fprintf(F, "<span class='chrG'>G</span>");break;
      case 'T':fprintf(F, "<span class='chrT'>T</span>");break;
      case 'N':fprintf(F, "<span class='chrN'>N</span>");break;
      default:break;
    }
  }
  for (i = t + 1; i < l; i++)
    fprintf(F, "<span class='chr_'>-</span>");
} /*psubstr2*/

/*碱基质量分布*/
void html_qualpic()
{
int i=0,j=0,k=0,s=0,t=0,f=0;
uli u=0ul,total=0ul,temp=0ul,tp=0ul;
float score=0;
fprintf(HTML,
"<div id='quality'>"
"  <div class='title'>Quality</div>");
for(i=0;i<=paired;i++){
  fprintf(HTML,
    "<div class='subtitle'>Read %d</div>",i+1);
  for(j=0;j<2;j++){
    fprintf(HTML,
    "  <div class='bqpic' id='pic_read%d_%s_qual'></div>"
    "  <script type='text/javascript'>",i,j?"after":"before");
    fprintf(HTML,"var read%d_%s_qual_total=[];",i,j?"after":"before");
    for(k=0;k<summary[i][j].maxlen;k++){
      fprintf(HTML,"var read%d_%s_p%d_qual=[];",i,j?"after":"before",k);
      int Max=0,Min=0;
      total=temp=0ul;
      for(s=0;s<MAX_QUALITY;s++){
        tp=0ul;
        for(t=0;t<5;t++){
          tp+=tstat[i][j][k][s][t];
        }
        if(tp>0ul){
          Min=s;
          break;
        }
      }
      for(s=MAX_QUALITY-1;s>=0;s--){
        tp=0ul;
        for(t=0;t<5;t++){
          tp+=tstat[i][j][k][s][t];
        }
        if(tp>0ul){Max=s;break;}/*记得修改*/
      }
     
      for(s=0;s<MAX_QUALITY;s++){
        for(t=0;t<5;t++){
          total+=tstat[i][j][k][s][t];
          //printf("%d:%d,%lu ",s,t,tstat[i][j][k][s][t]);
        }/*t*/
      }/*s*/
      //printf("\n 小:%d,大:%d \n\n\n",Min,Max);
      f=0;
      fprintf(HTML,"var read%d_%s_p%d_qual={y:[",i,j?"after":"before",k);
      fprintf(HTML,"%d,",Min);
      for(s=0;s<MAX_QUALITY;s++){
        temp+=tstat[i][j][k][s][0]+tstat[i][j][k][s][1]+tstat[i][j][k][s][2]+tstat[i][j][k][s][3]+tstat[i][j][k][s][4];
        score=(float)temp/total;
        if(score>=0.25 && (f&1)!=1){
          fprintf(HTML,"%d,",s);
          fprintf(HTML,"%d,",s);
          f|=1;
        }
        // if(score>=0.375 && (f&2)!=2){
        //   fprintf(HTML,"%d,",s+1);
        //   f|=2;
        // }
        if(score>=0.5 && (f&4)!=4){
          fprintf(HTML,"%d,",s);
          f|=4;
        }
        // if(score>=0.625 && (f&8)!=8){
        //   fprintf(HTML,"%d,",s+1);
        //   f|=8;
        // }
        if(score>=0.75 && (f&16)!=16){
          fprintf(HTML,"%d,",s);
          fprintf(HTML,"%d,",s);
          f|=16;
          break;
        }
      }/*s*/
      //printf("结束:%d\n",k);
      fprintf(HTML,"%d",Max);
      fprintf(HTML,"],"
      "type:'box',"
      "boxmean: false,"
      "name:'%d',"
      "line:{"
      "  outliercolor: '#CC3399',"
      "  outlierwidth: 1,"
      "  width:1"
      "},"
      "marker:{"
      "  color:'%s',"
      "  outliercolor: '#990066',"
      "  outlierwidth: 1,"
      "  symbol: '305'"
      "}"
      "};"
      ,k+1,QUALCOLOR[i*2+j]);
      //Math.max(  Math.min
      if(f>0)fprintf(HTML,"read%d_%s_qual_total.unshift(read%d_%s_p%d_qual);",i,j?"after":"before",i,j?"after":"before",k);
    }/*k*/
    
    fprintf(HTML,
    "var read%d_%s_qual_layout={"
    //"hovermode:false,"
    //"hoverinfo: 'none',"
    "  title:'Read%d quality scores(%s)',"
    "  titlefont: {"
    "    family:  \"Arail, \'Liberation Mono\', Menlo, Courier, monospace\","
    "    size: 14,"
    "    color: '#330033'"
    "  },"
    "  margin: {"
    "    l: 40,"
    "    r: 30,"
    "    b: 80,"
    "    t: 100"
    "  },"
    "  showlegend: false,"
    "  xaxis: {"
    "    title:'Position in read',"
    "    titlefont: {"
    "      family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "      size: 14,"
    "      color: '#7f7f7f'"
    "    },"
    "    autorange: true,"
    "    showgrid: true,"
    "    zeroline: true,"
    "    dtick: 4,"
    "    tickfont: {"
    "      family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "      size: 13,"
    "      color: 'black'"
    "    },"
    "    gridcolor: '#CCCCCC',"
    "    gridwidth: 1,"
    "    zerolinecolor: '#00FF00',"
    "    zerolinewidth: 1"
    "    "
    //"    zeroline: false"
    "  },"
    "  yaxis: {"
    "    title:'Quality',"
    "    titlefont: {"
    "     family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "      size: 14,"
    "      color: '#7f7f7f'"
    "    },"
    "    autorange: true,"
    "    showgrid: true,"
    "    zeroline: true,"
    "    dtick: 2,"
    "    tickfont: {"
    "      family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "      size: 13,"
    "      color: 'black'"
    "    },"
    "    gridcolor: '#CCCCCC',"
    "    gridwidth: 1,"
    "    zerolinecolor: '#00FF00',"
    "    zerolinewidth: 1"
    "    "
    "    "
    "  },"
    "  paper_bgcolor: 'rgb(243, 243, 243)',"
    "  plot_bgcolor: 'rgb(243, 243, 243)'"
    "};",i,j?"after":"before",i+1,j?"after":"before");

    fprintf(HTML,
    "Plotly.newPlot('pic_read%d_%s_qual', read%d_%s_qual_total,read%d_%s_qual_layout);"
    "</script>",i,j?"after":"before",i,j?"after":"before",i,j?"after":"before");  
  }/*j*/
}/*i*/
fprintf(HTML,
"<div class='subtitle'></div>"
"</div>");
} /*html_qualpic*/

/*碱基输出*/
void html_basepic()
{
int i=0,j=0,k=0,s=0,t=0,f=0;
uli u=0ul,total=0ul,temp=0ul;
float score=0;
fprintf(HTML,
"<div id='quality'>"
"  <div class='title'>Base</div>");
for(i=0;i<=paired;i++){
  fprintf(HTML,
    "<div class='subtitle'>Read %d</div>",i+1);
  for(j=0;j<2;j++){
    fprintf(HTML,
    "  <div class='bqpic2' id='pic_read%d_%s_base'></div>"
    "  <script type='text/javascript'>",i,j?"after":"before");
    fprintf(HTML,"var read%d_%s_base=[",i,j?"after":"before");
    for(t=0;t<5;t++){
      fprintf(HTML,"%s{x:[",t?",":"");
      for(k=0;k<summary[i][j].maxlen;k++){
        fprintf(HTML,"%s%d",k?",":"",k+1);
      }
      fprintf(HTML,"],");
      fprintf(HTML,"y:[");
      for(k=0;k<summary[i][j].maxlen;k++){
        total=temp=0;
        for(s=0;s<MAX_QUALITY;s++){
          temp+=tstat[i][j][k][s][t];
          total+=tstat[i][j][k][s][0]+tstat[i][j][k][s][1]+tstat[i][j][k][s][2]+tstat[i][j][k][s][3]+tstat[i][j][k][s][4];
        }/*s*/
        fprintf(HTML,"%s%.3f",k?",":"",total==0?0:(float)temp/total);
      }/*k*/
      fprintf(HTML,"],name: '%s',mode:'lines',line:{color:'%s',width:1.1}}",BASE[t],BASECOLOR[t]);
    }/*t*/
    fprintf(HTML,"];var pic_read%d_%s_layout={"
    "title:'Read%d base content(%s)',"
    "titlefont: {"
    "  family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "  size: 14,"
    "  color: '#7f7f7f'"
    "},"
    "margin: {"
    "  l: 30,"
    "  r: 0,"
    "  b: 40,"
    "  t: 30,"
    "  pad:0"
    "},"
    "xaxis:{"
    "dtick: 5,"
    "  title:'Position in read',"
    "  titlefont: {"
    "    family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "    size: 14,"
    "    color: '#7f7f7f'"
    "  },"
    "  gridcolor: '#CCCCCC',"
    "  gridwidth: 1,"
    "},"
    "yaxis:{"
    //"range: [0, 1],"
    "dtick: 0.1,"
    "  title:'',"
    "  gridcolor: '#CCCCCC',"
    "  gridwidth: 1,"
    "},"
    "height:400"
    "};"
    ,i,j?"after":"before",i+1,j?"after":"before");
    fprintf(HTML,"Plotly.newPlot('pic_read%d_%s_base', read%d_%s_base, pic_read%d_%s_layout);</script>",i,j?"after":"before",i,j?"after":"before",i,j?"after":"before");
  
  }/*j*/
}/*i*/
fprintf(HTML,
"<div class='subtitle'></div>"
"</div>");
}/*html_basepic*/

/*长度输出*/
void html_length()
{
int i=0,j=0,k=0,min=0,max=0;
fprintf(HTML,
"<div id='quality'>"
"  <div class='title'>Length</div>"
"  <div class='subtitle'>Distribution of sequence lengths</div>");
for(i=0;i<=paired;i++){
  for(j=0;j<2;j++){
    max=min=0;
    fprintf(HTML,
    "  <div class='bqpic3' id='pic_read%d_%s_length'></div>"
    "  <script type='text/javascript'>",i,j?"after":"before");
    fprintf(HTML,"var read%d_%s_length_data=[",i,j?"after":"before");
    for(k=0;k<summary[i][j].maxlen+10;k++){
      if(tlenstat[i][j][k]>0){
        min=k;
        break;      
      }
    }/*k*/
    for(k=summary[i][j].maxlen;k>=0;k--){
      if(tlenstat[i][j][k]>0){
        max=k;
        break;
      }
    }/*k*/
    fprintf(HTML,"{x:[");
    for(k=min-5;k<=max+5;k++){
      fprintf(HTML,"%s%d",k?",":"",k);
    }/*k*/
    fprintf(HTML,"],y:[");
    for(k=min-5;k<=max+5;k++){
      fprintf(HTML,"%s%lu",k?",":"",tlenstat[i][j][k]);
    }/*k*/
    fprintf(HTML,"],name: 'Read%d(%s)',type:'bar',line:{width:1}}",
    i+1,j?"after":"before",BASECOLOR[i*2+j]);
    fprintf(HTML,"];var read%d_%s_length_layout={"
    "title:'Read%d (%s)',"
    "titlefont: {"
    "  family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "  size: 14,"
    "  color: '#7f7f7f'"
    "},"
    "margin: {"
    "  l: 45,"
    "  r: 0,"
    "  b: 45,"
    "  t: 30,"
    "  pad:0"
    "},"
    "xaxis:{"
    "  dtick: 10,"
    "  title:'Position in read',"
    "  titlefont: {"
    "    family: \"Arail, 'Liberation Mono', Menlo, Courier, monospace\","
    "    size: 14,"
    "    color: '#7f7f7f'"
    "  },"
    "  gridcolor: '#CCCCCC',"
    "  gridwidth: 1,"
    "},"
    "yaxis:{"
    "  title:'',"
    "  gridcolor: '#CCCCCC',"
    "  gridwidth: 1,"
    "},"
    "height:250"
    "};",i,j?"after":"before",i+1,j?"after":"before");
    fprintf(HTML,"Plotly.newPlot('pic_read%d_%s_length',read%d_%s_length_data, read%d_%s_length_layout);</script>",
    i,j?"after":"before",i,j?"after":"before",i,j?"after":"before");
  }/*j*/
}/*i*/  
fprintf(HTML,
"<div class='subtitle'></div>"
"</div>");
}/*html_length()*/

/*输出命令*/
void html_cmd(char **argv){
fprintf(HTML,
"<div id='cmd'>"
"  <div class='title'>Command line</div>"
"  <span class='cmd'>");
char **p = argv;
while (*p) fprintf(HTML, "%s ", *p++);
fprintf(HTML,
"  </span>"
"</div>");
}/*html_tail()*/

/*输出html尾*/
void html_tail()
{
char tail[] =
"  </div>"
"</body>"
""
"</html>";
fprintf(HTML, tail);
}
