package hr.fer.zesoi.bioinf.lvn;

import hr.fer.zesoi.bioinf.fasta.FASTAParser;

import java.io.File;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		// File folder = new File("C:/Users/Gossamer/Desktop/Texts/");
		// File[] listOfFiles = folder.listFiles();
		//
		//
		//
		// String[] texts = new String[listOfFiles.length];
		//
		// for (int i = 0; i < texts.length; i++) {
		// texts[i] = listOfFiles[i].getAbsolutePath();
		// }
		//
		// String pattern = "C:/Users/Gossamer/Desktop/pattern.fa";
		// String output = "C:/Users/Gossamer/Desktop/output.txt";
		// int k = 4;
		//
		// Tests.performTests(texts, pattern, output, k);

		if (args.length != 3) {
			System.out.println("Invalid input.\nRequired format is:\n\ttextFilePath patternFilePath k");
			System.exit(-1);
		}

		if ((new File(args[0]).exists() == false)) {
			System.out.println("Given text file does not exist.");
			System.exit(-1);
		}

		if ((new File(args[1]).exists() == false)) {
			System.out.println("Given pattern file does not exist.");
			System.exit(-1);
		}

		int k = 0;
		try {
			k = Integer.parseInt(args[2]);
		} catch (NumberFormatException e) {
			System.out.println("Number format not recognised.\nPlease provide valid value for k.");
			System.exit(-1);
		}

		String text = FASTAParser.parseFASTAFile(args[0]).get(0).getSequence();
		String pattern = FASTAParser.parseFASTAFile(args[1]).get(0).getSequence();
		
		LVNAlgorithm lvn = new LVNAlgorithm(text, pattern, k);
		lvn.match();

	}

}
