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
    val T = 100
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
    for (i<- 0 to K-1)
          randomVector(i) = r.nextDouble()*W


    //normalization -- may use more concise way
    datasetND = normalization(datasetND)
    datasetQD = normalization(datasetQD)


    //generate hashTable TODO need to be convert to spark
    var hashMatrixN = Array.ofDim[Double](N, L)
    var hashMatrixQ = Array.ofDim[Double](Q, L)
    var n = 0
    var d = 0
    var l = 0
    for (n<-0 to N-1) {
      var hashFinalValue = 0.0
      for(l<-0 to L-1)
      for (k<-0 to K-1) {
        var dTemp = 0.0;
        for (d<-0 to D-1) {
          dTemp += datasetND(n)(d)*randomLine(l)(k)(d)
        }
        var hashvalue = math.floor(dTemp/W)
        hashFinalValue = hashvalue*randomVector(k) + hashFinalValue;
      }
      hashMatrixN(n)(l) = hashFinalValue
    }

    for (n<-0 to Q-1) {
      var hashFinalValue = 0.0
      for(l<-0 to L-1)
      for (k<-0 to K-1) {
        var dTemp = 0.0;
        for (d<-0 to D-1) {
          dTemp += datasetQD(n)(d)*randomLine(l)(k)(d)
        }
        var hashvalue = math.floor(dTemp/W)
        hashFinalValue = hashvalue*randomVector(k) + hashFinalValue;
      }
      hashMatrixQ(n)(l) = hashFinalValue
    }


    //generate collision matrix
    var collisionMatrix = Array.fill[Int](Q,N)(0)

    for (q<-0 to Q-1) {
      for (n<-0 to N-1){
      for (l<-0 to L-1) {
        if (hashMatrixN(n)(l) == hashMatrixQ(q)(l))
          collisionMatrix(q)(n)+=1
      }
    }
  }

    //generate Candidate set
    var candidateSet = Array.fill[Int](Q,0)(0)
    for (i<-0 to Q-1) {
      for (j<-0 to N-1) {
        if(collisionMatrix(i)(j)>=T)
          candidateSet(i):+j
      }
    }
  }


  def normalization(dataset:Array[Array[Double]]):Array[Array[Double]] ={
    var row = dataset.length
    var col = dataset(0).length
    var max = Array.fill[Double](col)(-9999)
    var min = Array.fill[Double](col)(9999)

    for (i<-0 to row-1)
      for (j<-0 to col-1) {
        if (dataset(i)(j) >= max(j))
          max(j) = dataset(i)(j)
        if (dataset(i)(j) <= min(j))
          min(j) = dataset(i)(j)
      }
    for (i<-0 to row-1) {
      for (j<-0 to col-1) {
        if(max(j)==min(j))
          dataset(i)(j) = 0.5;
        else
          dataset(i)(j) = (dataset(i)(j) - min(j))/(max(j)-min(j));
      }
    }
    return dataset
  }
}
