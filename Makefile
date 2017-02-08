SRC_DIR = src
export EXECUTABLE = members

$(EXECUTABLE):
	$(MAKE) -C $(SRC_DIR)
	mv $(SRC_DIR)/$@ .

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(EXECUTABLE)
