int main(){

	MemoryGame* mem = new MemoryGame();


	GUI* gui = new Gui();
	gui.start();
	mem.start();

	Connector* con = new Connector(&gui,&mem);








}