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

    val sparkSession = SparkSession.builder.master("local")
      .appName("spark session example")
      .getOrCreate()


    val df = sparkSession.read.option("header","false").
      csv("/home/peter/FYP/FastLSH/tests/dataset/dataset1000NoIndex.csv")


    var datasetQ = df.collect



    var i = 0;
    var j = 0;
    for(i <- datasetQ )
      for(j<-0 to (D-1))
    {
      println(i.get(j).toString.toDouble)

    }


  }




}
