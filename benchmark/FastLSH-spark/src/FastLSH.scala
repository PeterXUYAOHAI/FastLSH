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

    // parameter sets
    val D = 57
    val L = 200
    val K = 1
    val N = 1000
    val Q = 1000
    val W = 1.2
    val T = 100
    val r = scala.util.Random

    val sparkSession = SparkSession.builder.master("local")
      .appName("FastLSH")
      .getOrCreate()

    //generate randomLines
    var randomLine = Array.fill[Double](L, K, D)(r.nextGaussian())
    var randomVector = Array.fill[Double](K)(r.nextDouble()*W)

    //broadcast frequent used var
    sc.broadcast(randomLine)
    sc.broadcast(randomVector)

    //read in data
    val dfq = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")

    val dfn = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")

    //convert string to double
    var datasetQ = dfq.rdd.map(r=>(0 until D).map(r.getString(_).toDouble).toList)
    var datasetN = dfn.rdd.map(r=>(0 until D).map(r.getString(_).toDouble).toList)

    //cache the variables
    datasetN.cache()
    datasetQ.cache()

    //prepare for normalization
    var maxn = (0 until D).map(d=>datasetN.map(_(d)).max())
    var maxq = (0 until D).map(d=>datasetQ.map(_(d)).max())
    var minn = (0 until D).map(d=>datasetN.map(_(d)).min())
    var minq = (0 until D).map(d=>datasetQ.map(_(d)).min())

    //broadcast frequent used variable
    sc.broadcast(maxn)
    sc.broadcast(minn)

    //do normalization
    datasetN = datasetN
      .map(r=>(0 until D)
        .map(d=> if((maxn(d)-minn(d))>0) (r(d)-minn(d))/(maxn(d)-minn(d)) else 0.5).toList)

    //Q is small set, collect it and then broadcast it
    var datasetQB = datasetQ.collect().toList
    datasetQB = datasetQB
      .map(r=>(0 until D)
        .map(d=> if((maxq(d)-minq(d))>0) (r(d)-minq(d))/(maxq(d)-minq(d)) else 0.5).toList)

    //broadcast frequent used small and cache reused -- spark practice
    sc.broadcast(datasetQB)
    datasetN.cache()

    //get hash values
    var hashMatrixN = datasetN.map(n=>(0 until L)
      .map(l=>(0 until K)
        .map(k=>math.floor((0 until D)
        .map(d=>n(d)*randomLine(l)(k)(d))
        .reduce(_+_)/W)*randomVector(k))
        .reduce(_+_)))

    var hashMatrixQB = datasetQB.map(n=>(0 until L)
      .map(l=>(0 until K)
        .map(k=>math.floor((0 until D)
        .map(d=>n(d)*randomLine(l)(k)(d))
        .reduce(_+_)/W)*randomVector(k))
        .reduce(_+_)))

    //still cache...
    hashMatrixN.cache()

    //get the collisionMatrixes
    var collisionMatrix = hashMatrixN.map(n=>hashMatrixQB.map(q=>(0 until L)
      .map(l=>if(n(l)==q(l)) 1 else 0).reduce(_+_)))

    //rdd still cache..
    collisionMatrix.cache()

    //filter out candidate set
    var candidateSet = collisionMatrix.map(h=>(0 until N).filter(n=>(h(n)>T)))
    candidateSet.collect()

  }
}
