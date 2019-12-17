
$('.btn-success').on('click', function() {
    var $this = $(this);
    var html = $this.html();
    $this.button('loading').html(html + "<i class='fa fa-spinner fa-spin '></i> ");
});

$('.list-group-item-success').on('click', function() {
    var $this = $(this);
    var html = $this.html();
    $this.button('loading').html(html + "&nbsp;<i class='fa fa-spinner fa-spin '></i> ");
});