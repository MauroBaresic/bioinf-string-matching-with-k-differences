package hr.fer.zesoi.bioinf.test;

import hr.fer.zesoi.bioinf.fasta.FASTAParser;
import hr.fer.zesoi.bioinf.fasta.FASTASequence;
import hr.fer.zesoi.bioinf.lvn.LVNAlgorithm;

import java.io.FileWriter;
import java.io.IOException;

public class Tests {
	
	final public static int N = 10;

	public static void performTests(String[] textFilePaths, String patternFilePath, String outputFilePath,
			int k) {

		FASTASequence pattern = FASTAParser.parseFASTAFile(patternFilePath).get(0);
		StringBuilder sb = new StringBuilder();

		sb.append(String.format("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n", "textLength", "patternLength",
				"k-value", "elapsedTime", "memUsage"));
		String format = "%-12d\t%-12d\t%-12d\t%-12f\t%-12f";

		for (String filePath : textFilePaths) {
			FASTASequence text = FASTAParser.parseFASTAFile(filePath).get(0);
			LVNAlgorithm lvn = new LVNAlgorithm(text.getSequence(), pattern.getSequence(), k);

			long elapsedTime = 0;
			long peekMemory = 0;
			
			for (int i = 0; i < N; i++) {
				Runtime.getRuntime().gc();

				long startTime = System.currentTimeMillis();
				lvn.resetPeekMemoryUsage();

				lvn.match();

				elapsedTime += System.currentTimeMillis() - startTime;
				peekMemory += lvn.getPeekMemoryUsage();
			}
			
			elapsedTime /= N;
			peekMemory /= N;

			sb.append(
					String.format(format, text.getSequenceLength(), pattern.getSequenceLength(), k,
							elapsedTime * 1.0 / 1000, peekMemory * 1.0 / 1024 / 1024)).append("\n");
		}

		try {
			FileWriter writer = new FileWriter(outputFilePath);
			writer.write(sb.toString().replaceAll(",", "."));
			writer.flush();
			writer.close();
		} catch (IOException e) {
			System.out.println("Failed to write test results to file.");
		}

	}

}
