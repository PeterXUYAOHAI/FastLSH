import org.apache.spark.sql.SparkSession
import org.apache.spark.{SparkConf, SparkContext}

/**
  * Created by peter on 17-3-1.
  */
object FastLSH {

  def main(args: Array[String]) {
    val logFile = "/usr/lib/spark/README.md" // Should be some file on your system
    val conf = new SparkConf().setAppName("FastLSH").setMaster("local[*]")
    val sc = new SparkContext(conf)

    val D = 57
    val L = 200
    val K = 5
    val N = 1000
    val Q = 1000
    val W = 1.2
    val T = 100
    val r = scala.util.Random


    val sparkSession = SparkSession.builder.master("local")
      .appName("FastLSH")
      .getOrCreate()

    var randomLine = Array.fill[Double](L, K, D)(r.nextGaussian())
    var randomVector = Array.fill[Double](K)(r.nextDouble()*W)

    val dfq = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")

    val dfn = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")

    var datasetQ = dfq.rdd.map(r=>(0 until D).map(r.getString(_).toDouble).toList)
    var datasetN = dfn.rdd.map(r=>(0 until D).map(r.getString(_).toDouble).toList)

    var maxn = (0 until D).map(d=>datasetN.map(_(d)).max())
    var maxq = (0 until D).map(d=>datasetN.map(_(d)).max())
    var minn = (0 until D).map(d=>datasetN.map(_(d)).min())
    var minq = (0 until D).map(d=>datasetN.map(_(d)).min())

    val DI = List.range(0, D)
    val DD = DI.map(_.toDouble)
    val KI = List.range(0, K)
    val KD = KI.map(_.toDouble)

//    DD.foreach(println(_))
//    KD.foreach(println(_))

    datasetN = datasetN
      .map(r=>(0 until D)
        .map(d=> if((maxn(d)-minn(d))>0) (r(d)-minn(d))/(maxn(d)-minn(d)) else 0.5).toList)

    datasetQ = datasetQ
      .map(r=>(0 until D)
        .map(d=> if((maxq(d)-minq(d))>0) (r(d)-minq(d))/(maxq(d)-minq(d)) else 0.5).toList)

    println(randomLine.length)
    (0 until L-1).map(println(_))
    var hashMatrixN = datasetN.map(n=>(0 until 50).map(m=>KD
              .reduce((k1,k2)=>DD.reduce((d1,d2)=>
//                n(d1.toInt)
                //*
                 // randomLine (m)(0)(d1.toInt)
                d1
                //l.toInt
                   ))))


//
//    var hashMatrixN = datasetN.map(n=>(0 until L-1)
//      .map(l=>KD
//        .reduce((k1,k2)=>
//          math.floor(DD.reduce((d1,d2)=>n(d1.toInt)*
//            randomLine(l)(k1.toInt)(d1.toInt)
//            +n(d2.toInt)*randomLine(l)(k1.toInt)(d2.toInt))/W)*randomVector(k1.toInt)+
//          math.floor(DD.reduce((d1,d2)=>n(d1.toInt)*randomLine(l)(k2.toInt)(d1.toInt)
//            +n(d2.toInt)*randomLine(l)(k2.toInt)(d2.toInt))/W)*randomVector(k2.toInt)
//        )))


//    var hashMatrixQ = datasetQ.map(n=>(0 until L-1)
//      .map(l=>KD
//        .reduce((k1,k2)=>
//          math.floor(DD.reduce((d1,d2)=>n(d1.toInt)*randomLine(l)(k1.toInt)(d1.toInt)
//            +n(d2.toInt)*randomLine(l)(k1.toInt)(d2.toInt))/W)*randomVector(k1.toInt)+
//          math.floor(DD.reduce((d1,d2)=>n(d1.toInt)*randomLine(l)(k2.toInt)(d1.toInt)
//            +n(d2.toInt)*randomLine(l)(k2.toInt)(d2.toInt))/W)*randomVector(k2.toInt)
//        )))
//
//
//    var collisionMatrix = hashMatrixN.map(n=>hashMatrixQ.map(q=>(0 until L-1)
//      .reduce((l1,l2)=>(if(n(l1)==q(l1)) 1 else 0)+(if(n(l2)==q(l2)) 1 else 0))))


   hashMatrixN.foreach(_.map(println(_)))
//    //generate collision matrix
//    var collisionMatrix = Array.fill[Int](Q,N)(0)
//
//    for (q<-0 to Q-1) {
//      for (n<-0 to N-1){
//      for (l<-0 to L-1) {
//        if (hashMatrixN(n)(l) == hashMatrixQ(q)(l))
//          collisionMatrix(q)(n)+=1
//      }
//    }
//  }
//
//    //generate Candidate set
//    var candidateSet = Array.fill[Int](Q,0)(0)
//    for (i<-0 to Q-1) {
//      for (j<-0 to N-1) {
//        if(collisionMatrix(i)(j)>=T)
//          candidateSet(i):+j
//      }
//    }
//  }


//  def normalization(dataset:Array[Array[Double]]):Array[Array[Double]] ={
////    var row = dataset.length
////    var col = dataset(0).length
////    var max = Array.fill[Double](col)(-9999)
////    var min = Array.fill[Double](col)(9999)
////
////    for (i<-0 to row-1)
////      for (j<-0 to col-1) {
////        if (dataset(i)(j) >= max(j))
////          max(j) = dataset(i)(j)
////        if (dataset(i)(j) <= min(j))
////          min(j) = dataset(i)(j)
////      }
////    for (i<-0 to row-1) {
////      for (j<-0 to col-1) {
////        if(max(j)==min(j))
////          dataset(i)(j) = 0.5;
////        else
////          dataset(i)(j) = (dataset(i)(j) - min(j))/(max(j)-min(j));
////      }
////    }
////    return dataset
  }
}
