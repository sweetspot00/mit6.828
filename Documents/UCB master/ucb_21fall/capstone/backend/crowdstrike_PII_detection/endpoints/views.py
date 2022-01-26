from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse
from django.shortcuts import render
from django.views.generic import View
from .utils.word_extract import word_extract
from . import models


def uploadFile(request):
    if request.method == "POST":
        # Fetching the form data
        fileTitle = request.POST["fileTitle"]
        uploadedFile = request.FILES["uploadedFile"]
        extractor = word_extract()
        content = extractor.extract_from_pdf(uploadedFile)
        
        # go through model
        #
        # end
        score = 97
        return render(request, "upload-file-result.html", {'score': content})
    if request.method == "GET":
        return render(request, "upload-file.html")
