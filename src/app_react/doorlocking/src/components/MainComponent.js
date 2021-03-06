import React, { Component } from "react";
import { Route, Switch } from 'react-router-dom';
import Footer from './FooterComponent';
import Home from './HomeComponent';
import Menu from './MenuComponent';
import ListUsers from './ListUsersComponent';
import OpenDoor from './OpenDoorComponent';
import RegiserUser from './RegisterUserComponent';
import Logout from './LogoutComponent';
import Login from './LoginComponent';
import Config from './ConfigComponent';

class Main extends Component {

    constructor(props){
        super(props);
        this.layout = React.createRef();
        this.menus = [
            {link: '/open_door', name: 'Open Door'},
            {link: '/register_user', name: 'Register User'},
            {link: '/list_users', name: 'List/Change Users'},
            {link: '/config', name: 'Configuration'},
            {link: '/logout_handler', name: 'Logout'}
        ];
        this.heading = 'DoorLocking.App';
    }

    render(){
        return(
            <div id="layout" ref={this.layout}>
                <Menu layout={this.layout} menus={this.menus} heading={this.heading}/>
                <div id="main">
                    <Switch>
                        <Route path="/" exact component={Home}/>
                        <Route path="/login" component={Login}/>
                        <Route path="/open_door" component={OpenDoor}/>
                        <Route path="/register_user" component={RegiserUser}/>
                        <Route path="/list_users" component={ListUsers}/>
                        <Route path="/logout_handler" component={Logout}/>
                        <Route path="/config" component={Config}/>
                    </Switch>
                    <Footer/>
                </div>
            </div>
        );
    }
}


export default Main;