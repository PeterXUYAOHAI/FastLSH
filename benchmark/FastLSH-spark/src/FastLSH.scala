import org.apache.spark.sql.SparkSession
import org.apache.spark.{SparkConf, SparkContext}

/**
  * Created by peter on 17-3-1.
  */
object FastLSH {

  def main(args: Array[String]) {
    val logFile = "/usr/lib/spark/README.md" // Should be some file on your system
    val conf = new SparkConf().setAppName("Simple Application").setMaster("local[*]")
    val sc = new SparkContext(conf)

    val D = 57
    val L = 200
    val K = 1
    val N = 1000
    val Q = 1000
    val W = 1.2
    val r = scala.util.Random


    val sparkSession = SparkSession.builder.master("local")
      .appName("spark session example")
      .getOrCreate()


    var datasetQD = Array.ofDim[Double](Q,L)
    var datasetND = Array.ofDim[Double](N,L)
    var randomLine = Array.ofDim[Double](L, K, D)
    var randomVector = Array.ofDim[Double](K)


    //read in data
    var i = 0
    var j = 0
    var k = 0

    val dfq = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")
    var datasetQ = dfq.collectAsList


    val dfn = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")
    var datasetN = dfn.collectAsList()


    for(i <-0 to datasetQ.size()-1 )
      for(j<-0 to datasetQ.get(1).size-1)
        datasetQD(i)(j) = datasetQ.get(i)(j).toString.toDouble


    for(i <-0 to datasetN.size()-1 )
      for(j<-0 to datasetN.get(1).size-1)
        datasetND(i)(j) = datasetN.get(i)(j).toString.toDouble


    // generate randomLine
    for (i<- 0 to L-1)
      for (j<-0 to K-1)
        for (k<-0 to D-1)
          randomLine(i)(j)(k) = r.nextGaussian()


    // generate randomVector
    for (i<-0 to K-1)
      randomVector(i) = r.nextDouble()*W


    //generate hashTable
    



  }





}
