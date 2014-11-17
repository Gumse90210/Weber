<?php
include ("jpgraph/jpgraph.php");
include ("jpgraph/jpgraph_line.php");
include ("jpgraph/jpgraph_utils.inc.php");
    
include 'arduino_defines.php';

$n0 = ReadFileData::FromCSV(CHANNEL0_LOGFILE,$datay_0);
$n1 = ReadFileData::FromCSV(CHANNEL1_LOGFILE,$datay_1);
$n2 = ReadFileData::FromCSV(CHANNEL2_LOGFILE,$datay_2);
$n3 = ReadFileData::FromCSV(CHANNEL3_LOGFILE,$datay_3);
    
$n4 = ReadFileData::FromCSV(SETPOINT_LOGFILE,$datay_4);
$n5 = ReadFileData::FromCSV(FAN_LOGFILE,$datay_5);
  
//$avg_array = array($n0);//create avg array

if( $n0 == false ) {
    die("Can't read data.csv.");
}

// Setup the graph

$graph = new Graph(568,480);

$graph->SetColor('blue@0.9');

$graph->title->Set('History');
$graph->SetScale("intlin");
    
$l0 = new LinePlot($datay_0);
$l0->SetColor('red');
$l0->SetWeight(2);
$l0->SetLegend("Pit");
$graph->Add($l0);
    
$l1 = new LinePlot($datay_1);
$l1->SetColor('yellow');
$l1->SetWeight(2);
$l1->SetLegend("Food 1");
$graph->Add($l1);

$l2 = new LinePlot($datay_2);
$l2->SetColor('green');
$l2->SetWeight(2);
$l2->SetLegend("Food 2");
$graph->Add($l2);

$l3 = new LinePlot($datay_3);
$l3->SetColor('blue');
$l3->SetWeight(2);
$l3->SetLegend("Food 3");
$graph->Add($l3);

    $l4 = new LinePlot($datay_4);
    $l4->SetColor('white');
    $l4->SetWeight(2);
    $l4->SetLegend("SP");
    $graph->Add($l4);

    $l5 = new LinePlot($datay_5);
    $l5->SetColor('black');
    $l5->SetWeight(2);
    $l5->SetLegend("Fan");
    $graph->Add($l5);

    $graph->legend->SetLayout(LEGEND_HOR);
    $graph->legend->SetPos(0.5,0.98,'center','bottom');
    
 //Average
 //   $i = 0;
 //   while ($i < $n0)
 //   {
 //     $sum += $datay_0[$i];
 //     $i = $i + 1;
 //   }
 //
 //   //calc average
 //   $sum /= $n0;
 //
 //   //load into array
 //   $i = 0;
 //   while ($i < $n0)
 //   {
 //       $avg_array[$i] = $sum;
 //       $i = $i + 1;
 //   }
 //   //add line
 //   $l6 = new LinePlot($avg_array);
 //   $l6->SetColor('pink');
 //   $l6->SetStyle('solid');
 //   $l6->SetWeight(1);
 //   $l6->SetLegend("Avg");
 //   $graph->Add($l6);

//maximum line

//minimum line

// Get the handler to prevent the library from sending the
// image to the browser
$gdImgHandler = $graph->Stroke(_IMG_HANDLER);

// Default is PNG so use ".png" as suffix
$fileName = "myimage.png";
$graph->img->Stream($fileName);
 
// Send it back to browser
$graph->img->Headers();
$graph->img->Stream();

?>