package hr.fer.zesoi.bioinf.fasta;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FASTAParser {

	public static List<FASTASequence> parseFASTAFile(String fileName) {
		List<FASTASequence> sequences = new ArrayList<>();

		BufferedReader reader = null;

		try {
			reader = new BufferedReader(new FileReader(fileName));
			String line = null, name = null, description = null;
			StringBuilder sequence = new StringBuilder();
			boolean first = true;

			while ((line = reader.readLine()) != null) {
				line = line.trim();
				if (line.isEmpty()) {
					continue;
				}
				if (line.charAt(0) == '>') {
					if (first == false) {
						sequences.add(new FASTASequence(name, description, sequence.toString()));
						sequence = new StringBuilder();
					}

					first = false;
					int splitIndex = line.indexOf(" ");
					if (splitIndex != -1) {
						name = line.substring(1, splitIndex);
						description = line.substring(splitIndex + 1);
					} else {
						name = line.substring(1);
						description = "";
					}
					continue;
				}

				sequence.append(line);
			}
			sequences.add(new FASTASequence(name, description, sequence.toString()));

		} catch (FileNotFoundException e) {
			System.err.println("Couldn't open " + fileName + ".");
			System.exit(-1);
		} catch (IOException e) {
			System.err.println("Failed to read FASTA file line.");
		} finally {
			try {
				reader.close();
			} catch (IOException e) {
				System.err.println("Failed to close file reader.");
			}
		}

		return sequences;
	}

}
