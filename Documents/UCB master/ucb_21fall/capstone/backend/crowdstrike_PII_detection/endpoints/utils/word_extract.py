import PyPDF2 as p2

class word_extract():
    content = []
    def __init__(self) -> None:
        pass
    
    def extract_from_pdf(self, file):
        pdfread = p2.PdfFileReader(file)
        # Extract single page

        if pdfread.getIsEncrypted() :
            return False
        
        # Extract entire pdf
        
        for i in range(0, pdfread.getNumPages()):
            pageinfo = pdfread.getPage(i)
            self.content.append(pageinfo.extractText())
        return self.content
    
            

        
        
    