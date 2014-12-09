package hr.fer.zesoi.bioinf.lvn;

import java.io.File;
import java.util.List;

import hr.fer.zesoi.bioinf.fasta.CreateTestFile;
import hr.fer.zesoi.bioinf.fasta.FASTAParser;
import hr.fer.zesoi.bioinf.fasta.FASTASequence;
import hr.fer.zesoi.bioinf.test.Tests;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		FASTASequence text = FASTAParser.parseFASTAFile("C:/Users/Gossamer/Desktop/ecBakterija.fa").get(0);
//
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/pattern.fa", 1000, 1000, "prviTest",
//				" ");
//
//		FASTASequence pattern = FASTAParser.parseFASTAFile("C:/Users/Gossamer/Desktop/pattern.fa").get(0);
//
//		LVNAlgorithm lvn = new LVNAlgorithm(text.getSequence(), pattern.getSequence(), 5);
//		System.gc();
//		lvn.match();
//		System.out.println(lvn.getPeekMemoryUsage() / 1024);
		
//		String[] texts = {"C:/Users/Gossamer/Desktop/ecBakterija.fa"};
		
		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text5e1.fa", 100000, 50, "test1",
//				"len : 50");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text25e1.fa", 1000, 250, "test2",
//				"len : 250");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text1e3.fa", 700000, 1000, "test3",
//				"len : 1000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text5e3.fa", 770000, 5000, "test4",
//				"len : 5000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text1e4.fa", 23000, 10000, "test5",
//				"len : 10000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text5e4.fa", 543400, 50000, "test6",
//				"len : 50000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text1e5.fa", 3000000, 100000, "test7",
//				"len : 100000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text5e5.fa", 2100000, 500000, "test8",
//				"len : 500000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text1e6.fa", 30000, 1000000, "test9",
//				"len : 1000000");
//		
//		CreateTestFile.createTestFile(text, "C:/Users/Gossamer/Desktop/text2e6", 0, 2500000, "test10",
//				"len : 2500000");
		
		File folder = new File("C:/Users/Gossamer/Desktop/Texts/");
		File[] listOfFiles = folder.listFiles();
		
		String[] texts = new String[listOfFiles.length];
		for (int i = 0; i < texts.length; i++) {
			texts[i] = listOfFiles[i].toString();
		}
		
		String pattern = "C:/Users/Gossamer/Desktop/pattern.fa";
		String output = "C:/Users/Gossamer/Desktop/output.txt";
		int k = 4;
		
		Tests.performTests(texts, pattern, output, k);
		
		
		

		// System.out.println(sequences.get(0).toString().lastIndexOf("\n"));

	}

}
