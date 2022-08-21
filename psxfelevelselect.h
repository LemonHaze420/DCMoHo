#ifndef PSXFE_LEVEL_SELECT_H
#define PSXFE_LEVEL_SELECT_H

class	CPSXFELevelSelect
{
public:
	SINT	Run();

protected:
	BOOL	LoadData();
	
	void	Process();
	
	POLY_FT4	*DrawSprite(SINT n, SINT px, SINT py, SINT xs = 256, SINT ys = 256, SINT uvm = 0);
	void	Render();
	
	BOOL	mQuit;
	
};

extern CPSXFELevelSelect	LEVELSELECT;

#endif