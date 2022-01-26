from django.urls import path

from . import views
app_name = 'endpoints'
urlpatterns = [
    path("", views.uploadFile, name = "uploadFile"),
]